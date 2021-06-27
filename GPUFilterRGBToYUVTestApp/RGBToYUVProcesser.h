#ifndef RGBTOYUVPROCESSER_H
#define RGBTOYUVPROCESSER_H

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

class RGBToYUVProcesser : public QObject
{
    Q_OBJECT

public:
    RGBToYUVProcesser(QObject* parent = nullptr);
    ~RGBToYUVProcesser();

    void init(void);
    void render(void);
    void resize(int w, int h);

    void setImage(const QImage& image);
    QImage packImage(void);

private:
    void initCreate(void);

    QOffscreenSurface* m_pSurface = nullptr;
    QOpenGLContext* m_pContext = nullptr;

    GPUFilterTexture* m_pTexture = nullptr;
    GPUFilterPostProcessScene* m_pPostScene = nullptr;
    GPUFilterPBO2* m_pPBO = nullptr;
};

#endif
