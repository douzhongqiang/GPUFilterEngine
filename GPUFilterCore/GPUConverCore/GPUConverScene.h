#ifndef GPUCONVERSCENE_H
#define GPUCONVERSCENE_H

#include <QObject>
#include <QtOpenGL>
#include "OpenGLCore/GPUFilterCamera.h"
#include "OpenGLCore/GPUFilterRectMesh.h"
#include "OpenGLCore/GPUFilterShaderProgram.h"
#include "OpenGLCore/GPUFilterTool.h"
#include "OpenGLCore/GPUFilterTexture.h"
#include "GPUFilterCore_global.h"

class GPUFILTERCORE_EXPORT GPUConverScene : public QObject
{
    Q_OBJECT

public:
    enum Type{
        t_None,
        t_RGB,          // RGB888
        t_RGBA,

        t_YUV420P
    };

public:
    GPUConverScene(QObject* parent = nullptr);
    ~GPUConverScene();

    void setSrcData(Type type, const char* pData[], int width, int height);
    void setDestType(Type type);

    void init(void);
    void render(void);
    void resize(int w, int h);

private:
    GPUFilterShaderProgram* m_pShaderProgram = nullptr;
    GPUFilterRectMesh* m_imageMesh = nullptr;
    QVector<GPUFilterTexture*> m_textures;
    GPUFilterCamera* m_pCamera = nullptr;

    QVector<GPUFilterMesh*> m_meshs;

    Type m_srcType;
    Type m_destType;
    int m_nWidth = 0;
    int m_nHeight = 0;

    bool m_isInited = false;
    bool m_isCreated = false;
    void create(void);
};

#endif
