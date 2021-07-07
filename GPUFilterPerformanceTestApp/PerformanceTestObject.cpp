#include "PerformanceTestObject.h"
#include "GPUFilterVideoPlayerScene.h"
#include "PerformanceTestConverThread.h"
#include "widget.h"
#include <QApplication>
#include <QDebug>

PerformanceTestObject::PerformanceTestObject(QObject* parent)
    :QObject(parent)
{
    initTimer();
    initCreate();

    m_pVideoPlayerScene = new GPUFilterVideoPlayerScene(this);
    m_pFBO = new GPUFilterFBO(this);
    m_pPBO = new GPUFilterPBO2(this);

    // Conver Thread
    m_pConverThread = new PerformanceTestConverThread(this);
    QObject::connect(m_pConverThread, &PerformanceTestConverThread::sendValues, this, &PerformanceTestObject::onRecvValues);
}

PerformanceTestObject::~PerformanceTestObject()
{

}

void PerformanceTestObject::setParentWidget(Widget* pWidget)
{
    m_pParentWidget = pWidget;
}

QImage PerformanceTestObject::getCurrentImage(void)
{
    m_pContext->makeCurrent(m_pSurface);

    m_pFBO->bind();
    QImage image;
    m_pPBO->getImage(image);
    image = image.mirrored();
    m_pFBO->unbind();

    return image;
}

void PerformanceTestObject::startTest(void)
{
    m_pTimer->start();
    m_pConverThread->startConver();
}

void PerformanceTestObject::init(void)
{
    m_pContext->makeCurrent(m_pSurface);

    m_pParentWidget->appendText("Init Scene Start...");
    qApp->processEvents();

    m_pVideoPlayerScene->init();
    m_pFBO->create();
    m_pPBO->create(800, 600);

    QString currentPath = qApp->applicationDirPath() + "/";

    // Load Model
    m_pParentWidget->appendText("Loading Model...");
    qApp->processEvents();
    m_pVideoPlayerScene->loadModel(currentPath + "Indoor plant 3/Low-Poly Plant_.obj");

    // Load Animation Model
    m_pParentWidget->appendText("Loading Animation Model...");
    qApp->processEvents();
    m_pVideoPlayerScene->loadAnimationModel(currentPath + "vampire (2)/dancing_vampire.dae");

    m_pParentWidget->appendText("Scene Init Finished");
    qApp->processEvents();
}

void PerformanceTestObject::render(void)
{
    m_pContext->makeCurrent(m_pSurface);

    m_pFBO->bind();
    m_pVideoPlayerScene->render();
    m_pFBO->unbind();
}

void PerformanceTestObject::resize(int w, int h)
{
    m_pContext->makeCurrent(m_pSurface);

    m_pVideoPlayerScene->resize(w, h);
    m_pFBO->setFBOSize(w, h);
    m_pPBO->resize(w, h);
}

void PerformanceTestObject::setSceneSize(int w, int h)
{
    m_nWidth = w;
    m_nHeight = h;

    if (m_isInited)
    {
        this->resize(w, h);
    }
}

void PerformanceTestObject::getSceneSize(int& w, int& h)
{
    w = m_nWidth;
    h = m_nHeight;
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

void PerformanceTestObject::initTimer(void)
{
    m_pTimer = new QTimer(this);
    m_pTimer->setInterval(1000.0 / 15);
    QObject::connect(m_pTimer, &QTimer::timeout, this, &PerformanceTestObject::onTimeout);
}

void PerformanceTestObject::onTimeout(void)
{
    if (!m_isInited)
    {
        m_isInited = true;
        init();
        resize(m_nWidth, m_nHeight);
    }

    render();

    // For Test Save Image
    static int number = 0;
    QImage image = getCurrentImage();
    QString tempFrameString = "Created Image %1 Frame";
    m_pParentWidget->appendText(tempFrameString.arg(number++));
    qApp->processEvents();

    m_pConverThread->addImage(image);
}

void PerformanceTestObject::onRecvValues(qint64 currentValue, qint64 avgValue)
{
    QString str = "%1Mode: Current delayed is %2, Avg is %3";
    QString tempString = "GPU";
    if (!m_pConverThread->isUsedGPU())
    {
        tempString = "FFMpeg";
    }

    str = str.arg(tempString).arg(currentValue).arg(avgValue);
    m_pParentWidget->appendText(str);
    qApp->processEvents();
}

void PerformanceTestObject::setUsedGPU(bool isUsedGPU)
{
    m_pConverThread->setUsedGPU(isUsedGPU);
}
