#include "GPUFilterCamera.h"
#include <QtMath>

GPUFilterCamera::GPUFilterCamera(QObject* parent)
    :QObject(parent)
    , m_cameraPos(0.0f, 0.0f, 5.0f)
    , m_cameraFront(0.0f, 0.0f, -1.0f)
    , m_cameraUp(0.0f, 1.0f, 0.0f)
{

}

GPUFilterCamera::~GPUFilterCamera()
{

}

void GPUFilterCamera::keyPressEvent(QKeyEvent* event)
{
    float speed = 0.5;
    if (event->key() == Qt::Key_W || event->key() == Qt::Key_Up)
    {
        m_cameraPos += speed * m_cameraFront;

        emit cameraPostionChanged(m_cameraPos);
    }
    else if (event->key() == Qt::Key_S || event->key() == Qt::Key_Down)
    {
        m_cameraPos -= speed * m_cameraFront;

        emit cameraPostionChanged(m_cameraPos);
    }
    else if (event->key() == Qt::Key_A || event->key() == Qt::Key_Left)
    {
        QVector3D rightVec = QVector3D::crossProduct(m_cameraFront, m_cameraUp).normalized();
        m_cameraPos -= speed * rightVec;

        emit cameraPostionChanged(m_cameraPos);
    }
    else if (event->key() == Qt::Key_D || event->key() == Qt::Key_Right)
    {
        QVector3D rightVec = QVector3D::crossProduct(m_cameraFront, m_cameraUp).normalized();
        m_cameraPos += speed * rightVec;

        emit cameraPostionChanged(m_cameraPos);
    }
}

void GPUFilterCamera::mousePressEvent(QMouseEvent* event)
{
    m_bPressed = true;
    m_startPos = event->pos();
}

void GPUFilterCamera::mouseMoveEvent(QMouseEvent* event)
{
    if (!m_bPressed)
        return;

    float xoffset = event->x() - m_startPos.x();
    float yoffset = m_startPos.y() - event->y();
    m_startPos = event->pos();

    float sensitivity = 0.05f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    m_yaw   += xoffset;
    m_pitch += yoffset;

    QVector3D front;
    front.setX(qCos(qDegreesToRadians(m_yaw)) * qCos(qDegreesToRadians(m_pitch)));
    front.setY(qSin(qDegreesToRadians(m_pitch)));
    front.setZ(qSin(qDegreesToRadians(m_yaw)) * qCos(qDegreesToRadians(m_pitch)));

    m_cameraFront = front.normalized();
    emit cameraFrontChanged(m_cameraFront);
}

void GPUFilterCamera::mouseReleaseEvent(QMouseEvent* event)
{
    m_bPressed = false;
}

void GPUFilterCamera::wheelEvent(QWheelEvent *event)
{
    float interval = -event->delta() / 100.0;
    m_persAngle += interval;
    if (m_persAngle < 1.0f)
        m_persAngle = 1.0f;
    if (m_persAngle > 45.0f)
        m_persAngle = 45.0f;

    emit projMatrixAngleChanged(m_persAngle);
}

void GPUFilterCamera::setCameraPostion(const QVector3D& cameraPos)
{
    m_cameraPos = cameraPos;
}

void GPUFilterCamera::setCameraFront(const QVector3D& cameraFront)
{
    m_cameraFront = cameraFront;
}

void GPUFilterCamera::setCameraUp(const QVector3D& cameraUp)
{
    m_cameraUp = cameraUp;
}

QVector3D GPUFilterCamera::getCameraPostion(void)
{
    return m_cameraPos;
}

QVector3D GPUFilterCamera::getCameraFront(void)
{
    return m_cameraFront;
}

QVector3D GPUFilterCamera::getCameraUp(void)
{
    return m_cameraUp;
}

void GPUFilterCamera::setPersAngle(float angle)
{
    m_persAngle = angle;
}

float GPUFilterCamera::getPerAngle(void)
{
    return m_persAngle;
}

void GPUFilterCamera::setViewport(int width, int height)
{
    m_nViewportWidth = width;
    m_nViewportHeight = height;
}

void GPUFilterCamera::getViewport(int& width, int& height)
{
    width = m_nViewportWidth;
    height = m_nViewportHeight;
}

void GPUFilterCamera::setViewRange(float nNear, float nFar)
{
    m_nNear = nNear;
    m_nFar = nFar;
}

void GPUFilterCamera::getViewRange(float& nNear, float& nFar)
{
    nNear = m_nNear;
    nFar = m_nFar;
}

void GPUFilterCamera::reSetProjectMatrix(void)
{
    m_PMat.setToIdentity();
    if (m_isEnabled)
        m_PMat.perspective(m_persAngle, m_nViewportWidth * 1.0 / m_nViewportHeight, m_nNear, m_nFar);
}

void GPUFilterCamera::reSetViewMatrix(void)
{
    m_VMat.setToIdentity();
    if (m_isEnabled)
        m_VMat.lookAt(m_cameraPos, m_cameraPos + m_cameraFront, m_cameraUp);
}

QMatrix4x4 GPUFilterCamera::getVMatrix(void)
{
    return m_VMat;
}

QMatrix4x4 GPUFilterCamera::getPMatrix(void)
{
    return m_PMat;
}

void GPUFilterCamera::setCameraEnabled(bool isEnabled)
{
    m_isEnabled = isEnabled;
}

bool GPUFilterCamera::isCameraEnabled(void)
{
    return m_isEnabled;
}

void GPUFilterCamera::activeCamera(void)
{
    reSetProjectMatrix();
    reSetViewMatrix();
}
