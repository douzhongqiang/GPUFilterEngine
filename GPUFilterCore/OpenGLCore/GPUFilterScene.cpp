#include "GPUFilterScene.h"
#include "OpenGLCore/GPUFilterTool.h"

GPUFilterScene::GPUFilterScene(QObject* parent)
    :QObject(parent)
    , m_bgColor(0, 0, 0)
{

}

GPUFilterScene::~GPUFilterScene()
{
//    for (auto iter = m_meshVec.begin(); iter != m_meshVec.end(); ++iter)
//    {
//        if (*iter == nullptr)
//            continue;

//        (*iter)->setParent(nullptr);
//    }
}

void GPUFilterScene::init(void)
{
    if (!m_isInited)
    {
        // Create Shader Program
        if (m_pShaderProgram == nullptr)
        {
            m_pShaderProgram = new GPUFilterShaderProgram(this);
            m_pShaderProgram->create(":/shader/GeneralVertexShader.vsh", \
                                     ":/shader/GeneralFragmentShader.fsh");
        }

        // Create Camera
        if (m_pCamera == nullptr)
        {
            m_pCamera = new GPUFilterCamera(this);
        }

        // Mesh Init
        for (auto iter = m_meshVec.begin(); iter != m_meshVec.end(); ++iter)
        {
            GPUFilterMesh* pMesh = *iter;
            if (pMesh == nullptr || pMesh->hasCreated())
                continue;

            // Create VBO, IBO, VAO
            pMesh->create();
        }

        m_isInited = true;
    }
}

void GPUFilterScene::render(void)
{
    g_GPUFunc->glClearColor(m_bgColor.redF(), m_bgColor.greenF(), m_bgColor.blueF(), m_bgColor.alphaF());
    g_GPUFunc->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    g_GPUFunc->glEnable(GL_DEPTH_TEST);

    m_pShaderProgram->bind();

    effectLights();

    // Draw Mesh
    for (auto iter = m_meshVec.begin(); iter != m_meshVec.end(); ++iter)
    {
        GPUFilterMesh* pMesh = *iter;
        if (pMesh == nullptr)
            continue;

        // Set View And Projection Matrix
        m_pShaderProgram->getShaderProgram()->setUniformValue("V", m_pCamera->getVMatrix());
        m_pShaderProgram->getShaderProgram()->setUniformValue("P", m_pCamera->getPMatrix());
        // Set Camera Postion
        m_pShaderProgram->getShaderProgram()->setUniformValue("ViewPostion", m_pCamera->getCameraPostion());

        // Draw
        pMesh->draw();
    }

    m_pShaderProgram->unbind();
}

void GPUFilterScene::resize(int w, int h)
{
    g_GPUFunc->glViewport(0, 0, w, h);

    if (m_pCamera)
    {
        m_pCamera->setViewport(w, h);
        m_pCamera->activeCamera();
    }
}

void GPUFilterScene::addMesh(GPUFilterMesh* pMesh)
{
    if (pMesh == nullptr)
        return;

    pMesh->setCurrentScene(this);
    if (m_isInited)
        pMesh->create();

    m_meshVec.append(pMesh);
}

void GPUFilterScene::removeMesh(GPUFilterMesh* pMesh)
{
    if (pMesh == nullptr)
        return;

    pMesh->setCurrentScene(nullptr);
    m_meshVec.removeOne(pMesh);
}

// Add Lights
void GPUFilterScene::addLight(GPUFilterLightBase* pLight)
{
    if (pLight == nullptr)
        return;

    m_lightVec.append(pLight);
    pLight->setCurrentScene(this);
}

// Get Shader Program
QOpenGLShaderProgram* GPUFilterScene::getShaderProgram(void)
{
    if (m_pShaderProgram)
        return m_pShaderProgram->getShaderProgram();

    return nullptr;
}

// Get Camera
GPUFilterCamera* GPUFilterScene::getCamera(void)
{
    return m_pCamera;
}

void GPUFilterScene::effectLights(void)
{
    QOpenGLShaderProgram* pShaderProgram = m_pShaderProgram->getShaderProgram();
    pShaderProgram->setUniformValue("M_nLightCount", m_lightVec.size());

    int number = 0;
    for (auto iter = m_lightVec.begin(); iter != m_lightVec.end(); ++iter)
    {
        effectLights(*iter, number++);
    }
}

void GPUFilterScene::effectLights(GPUFilterLightBase* pLight, int index)
{
    if (pLight == nullptr)
        return;

    QOpenGLShaderProgram* pShaderProgram = m_pShaderProgram->getShaderProgram();
    pLight->processShader(pShaderProgram, index);
}

// Set/Get Background Color
void GPUFilterScene::setBgColor(const QColor& color)
{
    m_bgColor = color;
}

QColor GPUFilterScene::getBgColor(void)
{
    return m_bgColor;
}
