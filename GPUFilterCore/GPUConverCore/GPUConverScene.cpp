#include "GPUConverScene.h"
#include "OpenGLCore/GPUFilterTool.h"

GPUConverScene::GPUConverScene(QObject* parent)
    :QObject(parent)
    , m_srcType(t_None)
    , m_destType(t_None)
{

}

GPUConverScene::~GPUConverScene()
{

}

void GPUConverScene::setSrcData(Type type, const char* pData[], int width, int height)
{
    m_nWidth = width;
    m_nHeight = height;

    m_srcType = type;
    create();

    if (m_isCreated && type == t_RGB)
    {
        GPUFilterTexture* pTexture = m_textures[0];
        pTexture->setImageFormat(GPUFilterTexture::t_RGB);
        pTexture->setImageData((const char*)pData[0], width, height);
    }
}

void GPUConverScene::setDestType(Type type)
{
    m_destType = type;
}

void GPUConverScene::render(void)
{
    g_GPUFunc->glClearColor(0.4f, 0.4f, 0.4f, 1.0f);
    g_GPUFunc->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    if (m_pShaderProgram == nullptr)
        return;

    m_pShaderProgram->bind();

    for (int i=0; i<m_textures.size(); ++i)
    {
        GPUFilterTexture* pTexture = m_textures[i];
        pTexture->activeTexture(i);

        m_pShaderProgram->getShaderProgram()->setUniformValue("M_Texture", i);
    }

    // Draw Mesh
    if (m_imageMesh)
    {
        // Set View And Projection Matrix
        m_pShaderProgram->getShaderProgram()->setUniformValue("V", m_pCamera->getVMatrix());
        m_pShaderProgram->getShaderProgram()->setUniformValue("P", m_pCamera->getPMatrix());
        m_pShaderProgram->getShaderProgram()->setUniformValue("M", m_imageMesh->getModelMatrix());

        m_imageMesh->draw();
    }

    m_pShaderProgram->unbind();
}

void GPUConverScene::init(void)
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

        m_isInited = true;

        // Create Mesh And Texture
        create();
    }
}

void GPUConverScene::create(void)
{
    if (m_srcType == t_None || !m_isInited || m_isCreated)
        return;

    // Create Texture
    if (m_srcType == t_RGB)
    {
        GPUFilterTexture* pTexture = new GPUFilterTexture(this);
        pTexture->create();
        m_textures.append(pTexture);
    }

    // Create Mesh
    if (m_imageMesh == nullptr)
    {
        m_imageMesh = new GPUFilterRectMesh(this);
        m_imageMesh->init();

        // For Test
        QMatrix4x4 mat;
        mat.translate(0.0f, 0.0f, -80.0f);
        mat.scale(50.0f, 50.0f, 1.0f);

        m_imageMesh->setModelMartix(mat);
    }

    m_isCreated = true;
}

void GPUConverScene::resize(int w, int h)
{
    g_GPUFunc->glViewport(0, 0, w, h);

    if (m_pCamera)
    {
        m_pCamera->setViewport(w, h);
        m_pCamera->activeCamera();
    }
}
