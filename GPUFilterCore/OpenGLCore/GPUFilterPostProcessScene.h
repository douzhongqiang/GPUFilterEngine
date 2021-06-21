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
    GPUFilterPostProcessScene(QObject* parent = nullptr);
    ~GPUFilterPostProcessScene();

    virtual void init(void);
    virtual void render(void);
    virtual void resize(int w, int h);

    void attachScene(GPUFilterScene* pScene);
    void renderScene(void);

private:
    GPUFilterRectMesh* m_pMesh = nullptr;
    GPUFilterShaderProgram* m_pShaderProgram = nullptr;
    GPUFilterFBO* m_pFBO = nullptr;
    GPUFilterScene* m_pAttachedScene = nullptr;
};

#endif
