#include "GPUFilterPostProcessScene.h"
#include "GPUFilterShaderProgram.h"
#include "GPUFilterMesh.h"
#include "GPUFilterRectMesh.h"
#include "GPUFilterTexture.h"
#include "GPUFilterTool.h"
#include "GPUFilterFBO.h"
#include "GPUFilterScene.h"
#include <QOpenGLShader>

GPUFilterPostProcessScene::GPUFilterPostProcessScene(QObject* parent)
    :QObject(parent)
{
    m_pMesh = new GPUFilterRectMesh(this);
    m_pFBO = new GPUFilterFBO(this);
}

GPUFilterPostProcessScene::~GPUFilterPostProcessScene()
{

}

void GPUFilterPostProcessScene::init(void)
{
    // Create Shader Program
    if (m_pShaderProgram == nullptr)
    {
        m_pShaderProgram = new GPUFilterShaderProgram(this);
        m_pShaderProgram->create(":/shader/PostProcessVertexShader.vsh", \
                                 ":/shader/PostProcessVertexShader.fsh");
    }

    m_pMesh->init();
    m_pFBO->create();
}

void GPUFilterPostProcessScene::render(void)
{
    g_GPUFunc->glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    g_GPUFunc->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    m_pShaderProgram->bind();

    // Set Texture
    g_GPUFunc->glActiveTexture(GL_TEXTURE0);
    g_GPUFunc->glBindTexture(GL_TEXTURE_2D, m_pFBO->getTextureId());
    m_pShaderProgram->getShaderProgram()->setUniformValue("fboSample", 0);

    // Draw
    m_pMesh->draw();

    m_pShaderProgram->unbind();
}

void GPUFilterPostProcessScene::attachScene(GPUFilterScene* pScene)
{
    m_pAttachedScene = pScene;
}

void GPUFilterPostProcessScene::renderScene(void)
{
    m_pFBO->bind();
    if (m_pAttachedScene)
        m_pAttachedScene->render();
    m_pFBO->unbind();
}

void GPUFilterPostProcessScene::resize(int w, int h)
{
    m_pFBO->bind();
    g_GPUFunc->glViewport(0, 0, w, h);
    m_pFBO->setFBOSize(w, h);
    m_pFBO->unbind();
}
