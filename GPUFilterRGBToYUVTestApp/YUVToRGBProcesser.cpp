#include "YUVToRGBProcesser.h"
#include <QSurfaceFormat>
#include <QOffscreenSurface>
#include "OpenGLCore/GPUFilterTool.h"
#include "3DExtras/GPUFilterGeometryCubeBox.h"
#include "3DExtras/GPUFilterGeometryRect.h"

YUVToRGBProcesser::YUVToRGBProcesser(QObject* parent)
    :QObject(parent)
{
    initCreate();
}

YUVToRGBProcesser::~YUVToRGBProcesser()
{

}

void YUVToRGBProcesser::initCreate(void)
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
    m_pScene = new GPUFilterScene(this);
    // [3] Create Mesh And Material
    m_pMesh = new GPUFilterGeometryRect(this);
    QSharedPointer<GPUFilterMaterial> material(new GPUFilterMaterial);
    material->setColorEnabled(false);
    material->setLightEffect(false);
    material->setExtraTextureEnabled(true);
    m_pMesh->setMaterial(material);
    m_pMesh->setModelMartix(QMatrix4x4());
    // [4] Add To Scene
    m_pScene->addMesh(m_pMesh);

    // For Test
//    GPUFilterGeometryRect *pTestMesh = new GPUFilterGeometryRect(this);
//    m_pScene->addMesh(pTestMesh);

    // [5] Create FBO And POB
    m_pFBO = new GPUFilterFBO(this);
    m_pPBO = new GPUFilterPBO2(this);
}

void YUVToRGBProcesser::init(void)
{
    m_pContext->makeCurrent(m_pSurface);

    m_pScene->init();
    if (m_pScene->getCamera())
    {
        m_pScene->getCamera()->setCameraEnabled(false);
        m_pScene->getCamera()->activeCamera();
    }

    m_pFBO->create();
    m_pPBO->create(800, 600);
}

void YUVToRGBProcesser::render(void)
{
    m_pContext->makeCurrent(m_pSurface);

    m_pFBO->bind();
    m_pScene->render();
    m_pFBO->unbind();

    g_GPUFunc->glFlush();
    m_pContext->swapBuffers(m_pSurface);
}

void YUVToRGBProcesser::resize(int w, int h)
{
    m_pContext->makeCurrent(m_pSurface);

    m_pFBO->bind();

    g_GPUFunc->glViewport(0, 0, w, h);
    m_pFBO->setFBOSize(w, h);
    m_pScene->resize(w, h);

    m_pFBO->unbind();

    // Resize PBO
    m_pPBO->resize(w, h);
}

QImage YUVToRGBProcesser::packImage(void)
{
    m_pContext->makeCurrent(m_pSurface);

    m_pFBO->bind();
    QImage image;
    m_pPBO->getImage(image);
    m_pFBO->unbind();

    return image;
}

void YUVToRGBProcesser::setYData(const QByteArray& yData, int width, int height)
{
    m_pContext->makeCurrent(m_pSurface);

    // Set Y Data
    QSharedPointer<GPUFilterMaterial> pMaterial = m_pMesh->getMaterial();

    if (pMaterial->getExtraTexture1().isNull())
    {
        QSharedPointer<GPUFilterTexture> pYTexture(new GPUFilterTexture);
        pYTexture->setImageFormat(GPUFilterTexture::t_LUMINANCE);
        pMaterial->setExtraTexture1(pYTexture);
    }
    pMaterial->getExtraTexture1()->setImageData(yData.data(), width, height);

}

void YUVToRGBProcesser::setUData(const QByteArray& uData, int width, int height)
{
    m_pContext->makeCurrent(m_pSurface);

    QSharedPointer<GPUFilterMaterial> pMaterial = m_pMesh->getMaterial();

    // Set U Data
    if (pMaterial->getExtraTexture2().isNull())
    {
        QSharedPointer<GPUFilterTexture> pUTexture(new GPUFilterTexture);
        pUTexture->setImageFormat(GPUFilterTexture::t_LUMINANCE);
//        pUTexture->setImageData(yuvData[1].data(), width / 2, height / 2);
        pMaterial->setExtraTexture2(pUTexture);
    }
    pMaterial->getExtraTexture2()->setImageData(uData.data(), width / 2, height / 2);

}

void YUVToRGBProcesser::setVData(const QByteArray& vData, int width, int height)
{
    m_pContext->makeCurrent(m_pSurface);
    QSharedPointer<GPUFilterMaterial> pMaterial = m_pMesh->getMaterial();

    // Set V Data
    if (pMaterial->getExtraTexture3().isNull())
    {
        QSharedPointer<GPUFilterTexture> pVTexture(new GPUFilterTexture);
        pVTexture->setImageFormat(GPUFilterTexture::t_LUMINANCE);
//        pVTexture->setImageData(yuvData[2].data(), width / 2, height / 2);
        pMaterial->setExtraTexture3(pVTexture);
    }
    pMaterial->getExtraTexture3()->setImageData(vData.data(), width / 2, height / 2);

}
