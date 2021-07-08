#include "GPUFilterFrameConvertCore.h"
#include "OpenGLCore/GPUFilterTool.h"

GPUFilterFrameConvertCore::GPUFilterFrameConvertCore(QObject* parent)
    :QObject(parent)
{

}

GPUFilterFrameConvertCore::~GPUFilterFrameConvertCore()
{

}

void GPUFilterFrameConvertCore::rgb2yuv(AVFrame* rgbFrame, AVFrame* yuvFrame, bool isScale, int width, int height)
{
    QTime time;
    time.start();
    // [0] Init
    if (m_isFirstRender)
    {
        initCreate();
        init();
        m_isFirstRender = false;
    }

    m_pContext->makeCurrent(m_pSurface);

    // [1] Resize
    int nWidth = rgbFrame->width / 4;
    int nHeight = rgbFrame->height + rgbFrame->height / 2;
    if (isScale)
    {
        nWidth = width / 4;
        nHeight = height + height / 2;
    }
    this->resize(nWidth, nHeight);
    m_pPBO->setSrcSize(rgbFrame->width, rgbFrame->height);

    // [2] Set Texture
    if (rgbFrame->format == AV_PIX_FMT_RGB24)
    {
        m_pTexture->setImageFormat(GPUFilterTexture::t_RGB);
        m_pPostScene->setBRR0Visible(false);
    }
    else if (rgbFrame->format == AV_PIX_FMT_RGBA)
    {
        m_pTexture->setImageFormat(GPUFilterTexture::t_RGBA);
        m_pPostScene->setBRR0Visible(false);
    }
    else if (rgbFrame->format == AV_PIX_FMT_BGR0)
    {
        m_pTexture->setImageFormat(GPUFilterTexture::t_BGR0);
        m_pPostScene->setBRR0Visible(true);
    }
    m_pTexture->setImageData((const char*)rgbFrame->data[0], rgbFrame->width, rgbFrame->height);

    // [3] Render
    this->render();

    // [4] PackImage
    //this->packImage2((char*)yuvFrame->data[0], (char*)yuvFrame->data[1], (char*)yuvFrame->data[2]);
    QImage image = this->packImage();
    int n1 = time.elapsed();

    /*static int number = 0;
    QString imagePath = QString("%1/ConverImage2/%2.bmp").arg(qApp->applicationDirPath()).arg(number++);
    image.save(imagePath);*/

    // [5] Fill To Frame
    fillToFrame(image, yuvFrame);
    int n2 = time.elapsed();

    qDebug() << __FUNCTION__ << n1 << n2;
}

void GPUFilterFrameConvertCore::fillToFrame(const QImage& image, AVFrame* frame)
{
    int index = 0;

    qDebug() << image.width() << image.height();

    QImage tempImage1 = image.copy(QRect(0, 0, image.width(), frame->height));
    QImage tempImage2 = image.copy(QRect(0, frame->height, image.width() / 2, frame->height / 2));
    QImage tempImage3 = image.copy(QRect(image.width() / 2, frame->height, image.width() / 2, frame->height / 2));

    memcpy(frame->data[0], tempImage1.constBits(), frame->width * frame->height);
    memcpy(frame->data[1], tempImage2.constBits(), frame->width / 2 * frame->height / 2);
    memcpy(frame->data[2], tempImage3.constBits(), frame->width / 2 * frame->height / 2);

    return;
    // Copy Y Data
    for (int i = 0; i < frame->height; ++i)
    {
        int interval = i * frame->linesize[0];
        int srcInterval = i * frame->width;
        memset(frame->data[0] + interval, 0, frame->linesize[0]);
        memcpy(frame->data[0] + interval, image.constBits() + srcInterval, frame->width);
    }

    // Copy U Data
    index += frame->width * frame->height;
    for (int i = 0; i < frame->height / 2; ++i)
    {
        int interval = i * frame->linesize[1];
        int srcInterval = index + frame->width / 4 * 4 * i;

        memset(frame->data[1] + interval, 255 / 2, frame->linesize[1]);
        memcpy(frame->data[1] + interval, image.constBits() + srcInterval, frame->width / 2);
    }

    // Copy V Data
    //index += m_pFrame->width / 2 * m_pFrame->height / 2;
    for (int i = 0; i < frame->height / 2; ++i)
    {
        int interval = i * frame->linesize[2];
        int srcInterval = index + frame->width / 4 * 4 * i + frame->width / 4 * 4 / 2;

        memset(frame->data[2] + interval, 255 / 2, frame->linesize[2]);
        memcpy(frame->data[2] + interval, image.constBits() + srcInterval, frame->width / 2);
    }
}

void GPUFilterFrameConvertCore::initCreate(void)
{
    // [1] Create Context
    QSurfaceFormat format;
    format.setDepthBufferSize(24);
    format.setStencilBufferSize(8);
    format.setRedBufferSize(8);
    format.setBlueBufferSize(8);
    format.setGreenBufferSize(8);
    format.setAlphaBufferSize(8);
    format.setVersion(3, 3);
    format.setProfile(QSurfaceFormat::CompatibilityProfile);
    format.setOption(QSurfaceFormat::DeprecatedFunctions);
    format.setSwapBehavior(QSurfaceFormat::DoubleBuffer);
    format.setSwapInterval(1);

    m_pSurface = new QOffscreenSurface();
    m_pSurface->setFormat(format);
    m_pSurface->create();

    m_pContext = new QOpenGLContext(this);
    m_pContext->setFormat(format);
    m_pContext->create();
    m_pContext->makeCurrent(m_pSurface);
    m_pContext->functions()->initializeOpenGLFunctions();

    // [2] Create Scene
    m_pPostScene = new GPUFilterPostProcessScene(this);
    m_pPostScene->setPostProcessType(GPUFilterPostProcessScene::t_toYUV);

    // [3] Create Texture
    m_pTexture = new GPUFilterTexture(this);
    m_pTexture->setPBOEnabled(true);

    // [4] Create PBO
    m_pPBO = new GPUFilterPBO2(this);
    m_pPBO->setPBOSize(6);
}

void GPUFilterFrameConvertCore::init(void)
{
    m_pContext->makeCurrent(m_pSurface);

    m_pPostScene->init();
    m_pTexture->create();
    m_pPBO->create(800, 600, false);

    // Set Model Matrix
//    QMatrix4x4 mat;
//    mat.scale(1.0f, -1.0f, 1.0f);
//    m_pPostScene->getCurrentMesh()->setModelMartix(mat);
}

void GPUFilterFrameConvertCore::setImage(const QImage& image)
{
    m_pContext->makeCurrent(m_pSurface);
    m_pTexture->setImage(image);
}

QImage GPUFilterFrameConvertCore::packImage(void)
{
    m_pContext->makeCurrent(m_pSurface);

    m_pPostScene->getCurrentFBO()->bind();
    QImage image;
    m_pPBO->getImage(image);
    m_pPostScene->getCurrentFBO()->unbind();
    g_GPUFunc->glFlush();
    return image;
}

void GPUFilterFrameConvertCore::packImage2(char* pY, char* pU, char* pV)
{
    m_pContext->makeCurrent(m_pSurface);

    m_pPostScene->getCurrentFBO()->bind();
    m_pPBO->getImage(pY, pU, pV);
    m_pPostScene->getCurrentFBO()->unbind();
    g_GPUFunc->glFlush();
}

void GPUFilterFrameConvertCore::render(void)
{
    m_pContext->makeCurrent(m_pSurface);

    m_pPostScene->getCurrentFBO()->bind();
    m_pPostScene->setRenderTextureID(m_pTexture->getId());
    m_pPostScene->render();
    m_pPostScene->getCurrentFBO()->unbind();
}

void GPUFilterFrameConvertCore::resize(int w, int h)
{
    if (m_nWidth == w && m_nHeight == h)
        return;

    m_nWidth = w;
    m_nHeight = h;
    m_pContext->makeCurrent(m_pSurface);

    m_pPostScene->resize(w, h);
    m_pPBO->resize(w, h);
}
