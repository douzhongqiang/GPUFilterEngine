#include "RGBToYUVProcesser.h"
#include "OpenGLCore/GPUFilterTool.h"

RGBToYUVProcesser::RGBToYUVProcesser(QObject* parent)
    :QObject(parent)
{
    initCreate();
}

RGBToYUVProcesser::~RGBToYUVProcesser()
{

}

void RGBToYUVProcesser::initCreate(void)
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
    if (m_isToYUV)
        m_pPostScene->setPostProcessType(GPUFilterPostProcessScene::t_toYUV);
    else
        m_pPostScene->setPostProcessType(GPUFilterPostProcessScene::t_normal);

    // [3] Create Texture
    m_pTexture = new GPUFilterTexture(this);
    m_pTexture->setPBOEnabled(true);

    // [4] Create PBO
    m_pPBO = new GPUFilterPBO2(this);
}

void RGBToYUVProcesser::init(void)
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

void RGBToYUVProcesser::setImage(const QImage& image)
{
    m_pContext->makeCurrent(m_pSurface);
    m_pTexture->setImage(image);
}

QImage RGBToYUVProcesser::packImage(void)
{
    m_pContext->makeCurrent(m_pSurface);

    m_pPostScene->getCurrentFBO()->bind();
    QImage image;
    m_pPBO->getImage(image);
    m_pPostScene->getCurrentFBO()->unbind();
    g_GPUFunc->glFlush();
    return image;
}

void RGBToYUVProcesser::setRenderType(bool isToYUV)
{
    m_isToYUV = isToYUV;
}

void RGBToYUVProcesser::render(void)
{
    m_pContext->makeCurrent(m_pSurface);

    m_pPostScene->getCurrentFBO()->bind();
    m_pPostScene->setRenderTextureID(m_pTexture->getId());
    m_pPostScene->render();
    m_pPostScene->getCurrentFBO()->unbind();
}

void RGBToYUVProcesser::resize(int w, int h)
{
    if (m_nWidth == w && m_nHeight == h)
        return;

    m_nWidth = w;
    m_nHeight = h;
    m_pContext->makeCurrent(m_pSurface);

    m_pPostScene->resize(w, h);
    m_pPBO->resize(w, h);
}
