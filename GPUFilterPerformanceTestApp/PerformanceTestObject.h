#ifndef PERFORMANCETESTOBJECT_H
#define PERFORMANCETESTOBJECT_H

#include <QObject>
#include <QMatrix4x4>
#include <QtOpenGL>
#include <QOpenGLContext>
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
class PerformanceTestObject : public QObject
{
    Q_OBJECT

public:
    PerformanceTestObject(QObject* parent = nullptr);
    ~PerformanceTestObject();

    void init(void);
    void render(void);
    void resize(int w, int h);

private:
    void initCreate(void);

    QOffscreenSurface* m_pSurface = nullptr;
    QOpenGLContext* m_pContext = nullptr;

    GPUFilterVideoPlayerScene* m_pVideoPlayerScene = nullptr;
};

#endif
