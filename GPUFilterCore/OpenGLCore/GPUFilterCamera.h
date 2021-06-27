#ifndef GPUFILTERCAMERA_H
#define GPUFILTERCAMERA_H

#include <QObject>
#include <QOpenGLFunctions_2_0>
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLFunctions>
#include <QVector3D>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QMatrix4x4>
#include "GPUFilterCore_global.h"

class GPUFILTERCORE_EXPORT GPUFilterCamera : public QObject
{
    Q_OBJECT

public:
    GPUFilterCamera(QObject* parent = nullptr);
    ~GPUFilterCamera();

    void setViewport(int width, int height);
    void getViewport(int& width, int& height);

    void setViewRange(float nNear, float nFar);
    void getViewRange(float& nNear, float& nFar);

    // operate
    void keyPressEvent(QKeyEvent* event);
    void mousePressEvent(QMouseEvent* event);
    void mouseMoveEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent* event);
    void wheelEvent(QWheelEvent *event);

    // Camera Info
    void setCameraPostion(const QVector3D& cameraPos);
    void setCameraFront(const QVector3D& cameraFront);
    void setCameraUp(const QVector3D& cameraUp);

    QVector3D getCameraPostion(void);
    QVector3D getCameraFront(void);
    QVector3D getCameraUp(void);

    // Projection Matrix Info
    void setPersAngle(float angle);
    float getPerAngle(void);

    void activeCamera(void);

    QMatrix4x4 getVMatrix(void);
    QMatrix4x4 getPMatrix(void);

    void setCameraEnabled(bool isEnabled);
    bool isCameraEnabled(void);

private:
    QVector3D m_cameraPos;
    QVector3D m_cameraFront;
    QVector3D m_cameraUp;

    float m_yaw = -90.0f;               // yaw
    float m_pitch = 0.0f;               // pitch

    float m_persAngle = 45.0f;          // per angle

    QMatrix4x4 m_VMat;
    QMatrix4x4 m_PMat;

    QPoint m_startPos;
    bool m_bPressed = false;

    int m_nViewportWidth = 0;
    int m_nViewportHeight = 0;
    float m_nNear = 0.1f;
    float m_nFar = 100.0f;

    void reSetProjectMatrix(void);
    void reSetViewMatrix(void);

    bool m_isEnabled = true;

signals:
    void cameraPostionChanged(const QVector3D& cameraPos);
    void cameraFrontChanged(const QVector3D& front);
    void projMatrixAngleChanged(float angle);
};

#endif
