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
    , m_type(t_normal)
{
    m_pMesh = new GPUFilterRectMesh(this);
    m_pFBO = new GPUFilterFBO(this);
}

GPUFilterPostProcessScene::GPUFilterPostProcessScene(GPUFilterFBO* pFBO, QObject* parent)
    :QObject(parent)
    , m_type(t_normal)
    , m_pFBO(pFBO)
{
    m_pMesh = new GPUFilterRectMesh(this);
}

GPUFilterPostProcessScene::~GPUFilterPostProcessScene()
{

}

void GPUFilterPostProcessScene::init(bool needCreatePBO)
{
    // Create Shader Program
    if (m_pShaderProgram == nullptr)
    {
        m_pShaderProgram = new GPUFilterShaderProgram(this);
        m_pShaderProgram->create(":/shader/PostProcessVertexShader.vsh", \
                                 ":/shader/PostProcessVertexShader.fsh");
    }

    m_pMesh->init();

    if (needCreatePBO)
        m_pFBO->create();
}

void GPUFilterPostProcessScene::render(void)
{
    g_GPUFunc->glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    g_GPUFunc->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    g_GPUFunc->glEnable(GL_BLEND);
    g_GPUFunc->glBlendFunc(GL_ONE, GL_ZERO);

    m_pShaderProgram->bind();

    // Set Texture
    g_GPUFunc->glActiveTexture(GL_TEXTURE0);
    if (m_textureId == 0)
        g_GPUFunc->glBindTexture(GL_TEXTURE_2D, m_pFBO->getTextureId());
    else
        g_GPUFunc->glBindTexture(GL_TEXTURE_2D, m_textureId);

    m_pShaderProgram->getShaderProgram()->setUniformValue("m", m_pMesh->getModelMatrix());
    m_pShaderProgram->getShaderProgram()->setUniformValue("fboSample", 0);
    if (m_textureId == 0)
        m_pShaderProgram->getShaderProgram()->setUniformValue("width", m_pFBO->getFBOWidth());
    else
        m_pShaderProgram->getShaderProgram()->setUniformValue("width", m_pFBO->getFBOWidth() * 4);
    m_pShaderProgram->getShaderProgram()->setUniformValue("height", m_pFBO->getFBOHeight());
    m_pShaderProgram->getShaderProgram()->setUniformValue("m_PostProcessType", (int)m_type);
    m_pShaderProgram->getShaderProgram()->setUniformValue("m_isBGR0", m_isBGR0);
    
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

void GPUFilterPostProcessScene::setRenderTextureID(GLuint id)
{
    m_textureId = id;
}

GPUFilterRectMesh* GPUFilterPostProcessScene::getCurrentMesh(void)
{
    return m_pMesh;
}

GPUFilterFBO* GPUFilterPostProcessScene::getCurrentFBO(void)
{
    return m_pFBO;
}

void GPUFilterPostProcessScene::setPostProcessType(PostProcessType type)
{
    m_type = type;
}

void GPUFilterPostProcessScene::setBRR0Visible(bool isBGR0)
{
    m_isBGR0 = isBGR0;
}

bool GPUFilterPostProcessScene::isBGR0Visible(void)
{
    return m_isBGR0;
}
