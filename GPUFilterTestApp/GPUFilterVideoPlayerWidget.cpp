#include "GPUFilterVideoPlayerWidget.h"
#include "GPUFilterVideoPlayerScene.h"
#include "OpenGLCore/GPUFilterFBO.h"
#include "OpenGLCore/GPUFilterPostProcessScene.h"
#include "OpenGLCore/GPUFilterPBO2.h"
#include <QTimer>

GPUFilterVideoPlayerWidget::GPUFilterVideoPlayerWidget(QWidget* parent)
    :QOpenGLWidget(parent)
{
    this->setFocusPolicy(Qt::ClickFocus);
    this->setMouseTracking(true);

    m_pScene = new GPUFilterVideoPlayerScene(this);
    m_pPostProcessScene = new GPUFilterPostProcessScene(this);
    m_pPostProcessScene->attachScene(m_pScene);

    // Used To RGB TO YUV
    m_pYUVConvertScene = new GPUFilterPostProcessScene(m_pPostProcessScene->getCurrentFBO(), this);
    m_pYUVConvertScene->attachScene(m_pScene);

    m_pPackPBO = new GPUFilterPBO2(this);

    initTimer();
}

GPUFilterVideoPlayerWidget::~GPUFilterVideoPlayerWidget()
{

}

void GPUFilterVideoPlayerWidget::setYUVData(int type, const QVector<QByteArray>& yuvData, int width, int height)
{
    m_pScene->setYUVData(type, yuvData, width, height);

    this->update();
}

void GPUFilterVideoPlayerWidget::loadAnimationModel(const QString& modelFilePath)
{
    this->makeCurrent();
    m_pScene->loadAnimationModel(modelFilePath);

    this->update();
}

void GPUFilterVideoPlayerWidget::loadModel(const QString& modelFilePath)
{
    this->makeCurrent();
    m_pScene->loadModel(modelFilePath);

    this->update();
}

QImage GPUFilterVideoPlayerWidget::grapImage(int width, int height)
{
    this->makeCurrent();

    QImage image;
    m_pPostProcessScene->getCurrentFBO()->bind();
    m_pPackPBO->resize(width, height);
    m_pPackPBO->getImage(image);
    m_pPostProcessScene->getCurrentFBO()->unbind();

    return image;
}

void GPUFilterVideoPlayerWidget::initializeGL()
{
    this->initializeOpenGLFunctions();
    m_pScene->init();
    m_pPostProcessScene->init();
    m_pPackPBO->create(this->width(), this->height());

    return QOpenGLWidget::initializeGL();
}

void GPUFilterVideoPlayerWidget::resizeGL(int w, int h)
{
    m_pScene->resize(w, h);
    m_pPostProcessScene->resize(w, h);
    m_pPackPBO->resize(w, h);
    this->update();

    return QOpenGLWidget::resizeGL(w, h);
}

void GPUFilterVideoPlayerWidget::paintGL()
{
    //m_pScene->render();
    m_pPostProcessScene->render();
}

void GPUFilterVideoPlayerWidget::keyPressEvent(QKeyEvent* event)
{
    if (m_pScene && m_pScene->getCamera())
    {
        m_pScene->getCamera()->keyPressEvent(event);
        m_pScene->getCamera()->activeCamera();
    }

    this->update();
    return QOpenGLWidget::keyPressEvent(event);
}

void GPUFilterVideoPlayerWidget::mousePressEvent(QMouseEvent* event)
{
    if (m_pScene && m_pScene->getCamera())
    {
        m_pScene->getCamera()->mousePressEvent(event);
        m_pScene->getCamera()->activeCamera();
    }

    return QOpenGLWidget::mousePressEvent(event);
}

void GPUFilterVideoPlayerWidget::mouseMoveEvent(QMouseEvent* event)
{
    if (m_pScene && m_pScene->getCamera())
    {
        m_pScene->getCamera()->mouseMoveEvent(event);
        m_pScene->getCamera()->activeCamera();
    }
    this->update();

    return QOpenGLWidget::mouseMoveEvent(event);
}

void GPUFilterVideoPlayerWidget::mouseReleaseEvent(QMouseEvent* event)
{
    if (m_pScene && m_pScene->getCamera())
    {
        m_pScene->getCamera()->mouseReleaseEvent(event);
        m_pScene->getCamera()->activeCamera();
    }

    return QOpenGLWidget::mouseReleaseEvent(event);
}

void GPUFilterVideoPlayerWidget::wheelEvent(QWheelEvent *event)
{
    if (m_pScene && m_pScene->getCamera())
    {
        m_pScene->getCamera()->wheelEvent(event);
        m_pScene->getCamera()->activeCamera();
    }

    this->update();
    return QOpenGLWidget::wheelEvent(event);
}

void GPUFilterVideoPlayerWidget::onTimeout(void)
{
    this->makeCurrent();
    m_pPostProcessScene->renderScene();
    this->update();
}

void GPUFilterVideoPlayerWidget::initTimer(void)
{
    m_pTimer = new QTimer(this);
    m_pTimer->setInterval(0);
    QObject::connect(m_pTimer, &QTimer::timeout, this, &GPUFilterVideoPlayerWidget::onTimeout);

    m_pTimer->start();
}
