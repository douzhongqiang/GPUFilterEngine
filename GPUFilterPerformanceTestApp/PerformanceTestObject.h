#ifndef PERFORMANCETESTOBJECT_H
#define PERFORMANCETESTOBJECT_H

#include <QObject>
#include <QMatrix4x4>
#include <QtOpenGL>
#include <QOpenGLContext>
#include <QTimer>
#include "OpenGLCore/GPUFilterScene.h"
#include "OpenGLCore/GPUFilterMaterial.h"
#include "OpenGLCore/GPUFilterRectMesh.h"
#include "OpenGLCore/GPUFilterMesh.h"
#include "OpenGLCore/GPUFilterTexture.h"
#include "OpenGLCore/GPUFilterFBO.h"
#include "OpenGLCore/GPUFilterPBO2.h"
#include "OpenGLCore/GPUFilterCamera.h"
#include "OpenGLCore/GPUFilterPostProcessScene.h"

class GPUFilterVideoPlayerScene;
class Widget;
class PerformanceTestConverThread;

class PerformanceTestObject : public QObject
{
    Q_OBJECT

public:
    PerformanceTestObject(QObject* parent = nullptr);
    ~PerformanceTestObject();

    void init(void);
    void render(void);
    void resize(int w, int h);

    void setSceneSize(int w, int h);
    void getSceneSize(int& w, int& h);

    void setParentWidget(Widget* pWidget);
    QImage getCurrentImage(void);

    // Start Test
    void startTest(void);

    void setUsedGPU(bool isUsedGPU);
    void setResizeEnabled(bool isEnabled);
    void setResizeSize(int width, int height);

private:
    void initCreate(void);

    QOffscreenSurface* m_pSurface = nullptr;
    QOpenGLContext* m_pContext = nullptr;

    GPUFilterVideoPlayerScene* m_pVideoPlayerScene = nullptr;
    GPUFilterFBO* m_pFBO = nullptr;
    GPUFilterPBO2* m_pPBO = nullptr;

    Widget* m_pParentWidget = nullptr;
    PerformanceTestConverThread* m_pConverThread = nullptr;

    // Timer
    QTimer* m_pTimer = nullptr;
    void initTimer(void);
    bool m_isInited = false;

    int m_nWidth = 1280;
    int m_nHeight = 720;

    QImage m_tempImage;

private slots:
    void onTimeout(void);
    void onRecvValues(qint64 currentValue, qint64 avgValue);
};

#endif
