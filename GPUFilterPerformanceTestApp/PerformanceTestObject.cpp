#include "PerformanceTestObject.h"
#include "GPUFilterVideoPlayerScene.h"
#include <QApplication>
#include <QDebug>

PerformanceTestObject::PerformanceTestObject(QObject* parent)
    :QObject(parent)
{
    initCreate();
}

PerformanceTestObject::~PerformanceTestObject()
{

}

void PerformanceTestObject::init(void)
{
    m_pContext->makeCurrent(m_pSurface);

    qDebug() << "Init Scene Start...";

    m_pVideoPlayerScene->init();

    QString currentPath = qApp->applicationDirPath() + "/";

    // Load Model
    qDebug() << "Loading Model...";
    m_pVideoPlayerScene->loadModel(currentPath + "Indoor plant 3/Low-Poly Plant_.obj");

    // Load Animation Model
    qDebug() << "Loading Animation Model...";
    m_pVideoPlayerScene->loadAnimationModel(currentPath + "vampire (2)/dancing_vampire.dae");

    qDebug() << "Scene Init Finished";
}

void PerformanceTestObject::render(void)
{
    m_pContext->makeCurrent(m_pSurface);
    m_pVideoPlayerScene->render();
}

void PerformanceTestObject::resize(int w, int h)
{
    m_pContext->makeCurrent(m_pSurface);
    m_pVideoPlayerScene->resize(w, h);
}

void PerformanceTestObject::initCreate(void)
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
}
