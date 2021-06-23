#ifndef GPUFILTERPOSTPROCESSSCENE_H
#define GPUFILTERPOSTPROCESSSCENE_H

#include <QObject>
#include "GPUFilterCore_global.h"

class GPUFilterRectMesh;
class GPUFilterShaderProgram;
class GPUFilterFBO;
class GPUFilterScene;

class GPUFILTERCORE_EXPORT GPUFilterPostProcessScene : public QObject
{
    Q_OBJECT
        
public:
    enum PostProcessType
    {
        t_normal,
        t_toYUV
    };

public:
    GPUFilterPostProcessScene(QObject* parent = nullptr);
    GPUFilterPostProcessScene(GPUFilterFBO* pFBO, QObject* parent = nullptr);
    ~GPUFilterPostProcessScene();

    virtual void init(bool needCreatePBO = true);
    virtual void render(void);
    virtual void resize(int w, int h);

    int setRenderSize(int width, int height);

    void setPostProcessType(PostProcessType type);

    void attachScene(GPUFilterScene* pScene);
    void renderScene(void);

    GPUFilterFBO* getCurrentFBO(void);

private:
    GPUFilterRectMesh* m_pMesh = nullptr;
    GPUFilterShaderProgram* m_pShaderProgram = nullptr;
    GPUFilterFBO* m_pFBO = nullptr;
    GPUFilterScene* m_pAttachedScene = nullptr;

    PostProcessType m_type;
};

#endif
