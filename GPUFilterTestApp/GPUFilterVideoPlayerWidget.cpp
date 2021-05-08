#include "GPUFilterVideoPlayerWidget.h"
#include "GPUFilterVideoPlayerScene.h"

GPUFilterVideoPlayerWidget::GPUFilterVideoPlayerWidget(QWidget* parent)
    :QOpenGLWidget(parent)
{
    this->setFocusPolicy(Qt::ClickFocus);
    this->setMouseTracking(true);

    m_pScene = new GPUFilterVideoPlayerScene(this);
}

GPUFilterVideoPlayerWidget::~GPUFilterVideoPlayerWidget()
{

}

void GPUFilterVideoPlayerWidget::setYUVData(int type, const QVector<QByteArray>& yuvData, int width, int height)
{
    m_pScene->setYUVData(type, yuvData, width, height);

    this->update();
}

void GPUFilterVideoPlayerWidget::initializeGL()
{
    this->initializeOpenGLFunctions();
    m_pScene->init();

    return QOpenGLWidget::initializeGL();
}

void GPUFilterVideoPlayerWidget::resizeGL(int w, int h)
{
    m_pScene->resize(w, h);
    return QOpenGLWidget::resizeGL(w, h);
}

void GPUFilterVideoPlayerWidget::paintGL()
{
    m_pScene->render();
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
