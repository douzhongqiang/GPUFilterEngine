#ifndef YUVTORGBPROCESSER_H
#define YUVTORGBPROCESSER_H

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

class YUVToRGBProcesser : public QObject
{
    Q_OBJECT

public:
    YUVToRGBProcesser(QObject* parent = nullptr);
    ~YUVToRGBProcesser();

    void initCreate(void);

    void init(void);
    void render(void);
    void resize(int w, int h);

    void setYData(const QByteArray& yData, int width, int height);
    void setUData(const QByteArray& uData, int width, int height);
    void setVData(const QByteArray& vData, int width, int height);

    QImage packImage(void);

private:
    QOffscreenSurface* m_pSurface = nullptr;
    QOpenGLContext* m_pContext = nullptr;

    GPUFilterScene* m_pScene = nullptr;
    GPUFilterMesh* m_pMesh = nullptr;

    GPUFilterFBO* m_pFBO = nullptr;
    GPUFilterPBO2* m_pPBO = nullptr;
};

#endif
