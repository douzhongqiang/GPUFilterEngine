#include "GPUFilterMesh.h"
#include "GPUFilterTool.h"
#include "GPUFilterScene.h"
#include <QOpenGLShaderProgram>

GPUFilterMesh::GPUFilterMesh(QObject* parent)
    :QObject(parent)
{

}

GPUFilterMesh::~GPUFilterMesh()
{
    release();
}

// Add Postion
void GPUFilterMesh::addVertexPostion(const QVector3D& postion)
{
    m_posVec.push_back(postion);
}

// Add Color
void GPUFilterMesh::addColor(const QVector3D& color)
{
    m_colorVec.push_back(color);
}

// Add Normal
void GPUFilterMesh::addNormal(const QVector3D& normal)
{
    m_normalVec.push_back(normal);
}

// Add Coord
void GPUFilterMesh::addCoord(const QVector3D& coord)
{
    m_coordVec.push_back(coord);
}

void GPUFilterMesh::createIndices(void)
{
    if (m_hasSetedIndices)
        return;

    m_indices.clear();
    for (int i=0; i<m_posVec.size(); ++i)
        m_indices << i;
}

void GPUFilterMesh::setIndices(const QVector<GLuint>& indices)
{
    m_indices = indices;
    m_hasSetedIndices = true;
}

void GPUFilterMesh::setModelMartix(const QMatrix4x4& mat)
{
    m_MMatrix = mat;
}

// get Model Matrix
QMatrix4x4 GPUFilterMesh::getModelMatrix(void)
{
    return m_MMatrix;
}

bool GPUFilterMesh::create(void)
{
    m_hasCreated = true;

    createVAO();
    createVBO();
    createIBO();

    g_GPUFunc->glBindVertexArray(0);

    // Create Material Texture
    processMaterialTexture();

    return true;
}

void GPUFilterMesh::customVBODataCreate(int index)
{
    Q_UNUSED(index)
}

int GPUFilterMesh::customVBOSize(void)
{
    return 0;
}

bool GPUFilterMesh::hasCreated(void)
{
    return m_hasCreated;
}

void GPUFilterMesh::customMapVBOData(char* ptr)
{
    Q_UNUSED(ptr)
}

void GPUFilterMesh::createVBO(void)
{
    int nVBOSize = m_posVec.size() + m_colorVec.size() + m_normalVec.size() + m_coordVec.size();
    nVBOSize *= sizeof(QVector3D);
    nVBOSize += customVBOSize();

    // Create VBO
    g_GPUFunc->glGenBuffers(1, &m_nVBO);
    g_GPUFunc->glBindBuffer(GL_ARRAY_BUFFER, m_nVBO);
    g_GPUFunc->glBufferData(GL_ARRAY_BUFFER, nVBOSize, 0, GL_STATIC_DRAW);
    g_GPUFunc->glBindBuffer(GL_ARRAY_BUFFER, 0);

    // Bind Data
    g_GPUFunc->glBindBuffer(GL_ARRAY_BUFFER, m_nVBO);
    void* ptr = g_GPUFunc->glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
    if (ptr == nullptr)
    {
        g_GPUFunc->glBindBuffer(GL_ARRAY_BUFFER, 0);
        return;
    }

    int index = 0;
    memcpy((char*)ptr + index, m_posVec.data(), sizeof (QVector3D) * m_posVec.size());
    index += m_posVec.size() * sizeof (QVector3D);
    memcpy((char*)ptr + index, m_colorVec.data(), sizeof (QVector3D) * m_colorVec.size());
    index += m_colorVec.size() * sizeof (QVector3D);
    memcpy((char*)ptr + index, m_normalVec.data(), sizeof (QVector3D) * m_normalVec.size());
    index += m_normalVec.size() * sizeof (QVector3D);
    memcpy((char*)ptr + index, m_coordVec.data(), sizeof (QVector3D) * m_coordVec.size());
    index += m_coordVec.size() * sizeof(QVector3D);
    customMapVBOData((char*)ptr + index);
    g_GPUFunc->glUnmapBuffer(GL_ARRAY_BUFFER);
    g_GPUFunc->glBindBuffer(GL_ARRAY_BUFFER, 0);

    // Set Attribute
    g_GPUFunc->glBindBuffer(GL_ARRAY_BUFFER, m_nVBO);

    // Set Pos Attribute
    index = 0;
    g_GPUFunc->glEnableVertexAttribArray((int)t_postion);
    g_GPUFunc->glVertexAttribPointer((int)t_postion, 3, GL_FLOAT, GL_FALSE, sizeof(QVector3D), (void*)index);

    // Set Color Attribute
    index += sizeof(QVector3D) * m_posVec.size();
    g_GPUFunc->glEnableVertexAttribArray((int)t_color);
    g_GPUFunc->glVertexAttribPointer((int)t_color, 3, GL_FLOAT, GL_FALSE, sizeof(QVector3D), (void*)index);

    // Set Normal Attribute
    index += sizeof(QVector3D) * m_colorVec.size();
    g_GPUFunc->glEnableVertexAttribArray((int)t_normal);
    g_GPUFunc->glVertexAttribPointer((int)t_normal, 3, GL_FLOAT, GL_FALSE, sizeof(QVector3D), (void*)index);

    // Set coord Attribute
    index += sizeof(QVector3D) * m_normalVec.size();
    g_GPUFunc->glEnableVertexAttribArray((int)t_coord);
    g_GPUFunc->glVertexAttribPointer((int)t_coord, 3, GL_FLOAT, GL_FALSE, sizeof(QVector3D), (void*)index);

    // Create Custom VBO Data
    index += sizeof(QVector3D) * m_coordVec.size();
    customVBODataCreate(index);

    g_GPUFunc->glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void GPUFilterMesh::createIBO(void)
{
    createIndices();

    // Create IBO
    g_GPUFunc->glGenBuffers(1, &m_nIBO);
    g_GPUFunc->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_nIBO);
    g_GPUFunc->glBindBuffer(GL_ARRAY_BUFFER, m_nVBO);
    g_GPUFunc->glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(GLuint), m_indices.data(), GL_STATIC_DRAW);
//    g_GPUFunc->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
//    g_GPUFunc->glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void GPUFilterMesh::createVAO(void)
{
    g_GPUFunc->glGenVertexArrays(1, &m_nVAO);
    g_GPUFunc->glBindVertexArray(m_nVAO);
}

void GPUFilterMesh::release(void)
{
    g_GPUFunc->glDeleteBuffers(1, &m_nVBO);
    g_GPUFunc->glDeleteBuffers(1, &m_nIBO);

    g_GPUFunc->glDeleteVertexArrays(1, &m_nVAO);
}

void GPUFilterMesh::addTexture(QSharedPointer<GPUFilterTexture> pTexture)
{
    m_textures.append(pTexture);
}

void GPUFilterMesh::draw(void)
{
    if (!m_isVisible)
        return;

    QOpenGLShaderProgram* pShaderProgram = nullptr;

    // Set Model Matrix
    GPUFilterScene* pScene = getCurrentScene();
    if (pScene)
        pShaderProgram = pScene->getShaderProgram();
    pShaderProgram->setUniformValue("M", this->getModelMatrix());
    pShaderProgram->setUniformValue("M_isAnimation", m_isAnimation);

    // Draw Before Do Something
    drawBefore();

    // Process Material
    processMaterial();

    // Bind VAO And Draw
    g_GPUFunc->glBindVertexArray(m_nVAO);
    g_GPUFunc->glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, 0);
    g_GPUFunc->glBindVertexArray(0);

    // Draw After Do Something
    drawAfter();
}

void GPUFilterMesh::drawBefore(void)
{

}

void GPUFilterMesh::drawAfter(void)
{

}

// set/get Material
void GPUFilterMesh::setMaterial(QSharedPointer<GPUFilterMaterial> pMaterial)
{
    if (m_pMaterial == pMaterial)
        return;

    if (!m_pMaterial.isNull())
    {
        QObject::disconnect(m_pMaterial.get(), &GPUFilterMaterial::ambientTextureChanged, this, &GPUFilterMesh::onAmbientTextureChanged);
        QObject::disconnect(m_pMaterial.get(), &GPUFilterMaterial::diffuesTextureChanged, this, &GPUFilterMesh::onDiffuesTextureChanged);
        QObject::disconnect(m_pMaterial.get(), &GPUFilterMaterial::specularTextureChanged, this, &GPUFilterMesh::onSpecularTextureChanged);

        QObject::disconnect(m_pMaterial.get(), &GPUFilterMaterial::extraTexture1Changed, this, &GPUFilterMesh::onExtraTexture1Changed);
        QObject::disconnect(m_pMaterial.get(), &GPUFilterMaterial::extraTexture2Changed, this, &GPUFilterMesh::onExtraTexture2Changed);
        QObject::disconnect(m_pMaterial.get(), &GPUFilterMaterial::extraTexture3Changed, this, &GPUFilterMesh::onExtraTexture3Changed);
    }

    m_pMaterial = pMaterial;
    QObject::connect(m_pMaterial.get(), &GPUFilterMaterial::ambientTextureChanged, this, &GPUFilterMesh::onAmbientTextureChanged);
    QObject::connect(m_pMaterial.get(), &GPUFilterMaterial::diffuesTextureChanged, this, &GPUFilterMesh::onDiffuesTextureChanged);
    QObject::connect(m_pMaterial.get(), &GPUFilterMaterial::specularTextureChanged, this, &GPUFilterMesh::onSpecularTextureChanged);

    QObject::connect(m_pMaterial.get(), &GPUFilterMaterial::extraTexture1Changed, this, &GPUFilterMesh::onExtraTexture1Changed);
    QObject::connect(m_pMaterial.get(), &GPUFilterMaterial::extraTexture2Changed, this, &GPUFilterMesh::onExtraTexture2Changed);
    QObject::connect(m_pMaterial.get(), &GPUFilterMaterial::extraTexture3Changed, this, &GPUFilterMesh::onExtraTexture3Changed);
}

QSharedPointer<GPUFilterMaterial> GPUFilterMesh::getMaterial(void)
{
    return m_pMaterial;
}

// Get/Set Scene
void GPUFilterMesh::setCurrentScene(GPUFilterScene* pScene)
{
    m_pScene = pScene;
}

GPUFilterScene* GPUFilterMesh::getCurrentScene(void)
{
    return m_pScene;
}

// Process Material
void GPUFilterMesh::processMaterial(void)
{
    // Get Shader Program
    QOpenGLShaderProgram* pShaderProgram = nullptr;
    GPUFilterScene* pScene = getCurrentScene();
    if (pScene)
        pShaderProgram = pScene->getShaderProgram();
    if (pShaderProgram == nullptr)
        return;

    if (m_pMaterial.isNull())
    {
        pShaderProgram->setUniformValue("objectMaterial.enabled", false);
        return;
    }

    pShaderProgram->setUniformValue("objectMaterial.enabled", true);

    // Set Color Enabled
    bool isEnabledColor = m_pMaterial->getColorEnabled();
    pShaderProgram->setUniformValue("objectMaterial.enabledColor", isEnabledColor);
    // Set Shininess
    pShaderProgram->setUniformValue("objectMaterial.shininess", m_pMaterial->getShininess());
    // Set Light Effect
    pShaderProgram->setUniformValue("objectMaterial.lightEffect", m_pMaterial->isLightEffect());
    // set factor
    pShaderProgram->setUniformValue("M_factor", m_pMaterial->getFactor());

    if (isEnabledColor)
    {
        // Set Color Material
        pShaderProgram->setUniformValue("objectMaterial.ambientColor", m_pMaterial->getAmbientColor());
        pShaderProgram->setUniformValue("objectMaterial.diffuseColor", m_pMaterial->getDiffuesColor());
        pShaderProgram->setUniformValue("objectMaterial.specularColor", m_pMaterial->getSpecularColor());
    }
    else
    {
        // Set Texture Material

        // Set Ambient Texture
        QSharedPointer<GPUFilterTexture> pAmbientTexture = m_pMaterial->getAmbientTexture();
        if (pAmbientTexture)
        {
            pAmbientTexture->activeTexture(0);
            pShaderProgram->setUniformValue("objectMaterial.ambient", 0);
        }

        // Set Diffues Texture
        QSharedPointer<GPUFilterTexture> pDiffuseTexture = m_pMaterial->getDiffuesTexture();
        if (pDiffuseTexture)
        {
            pDiffuseTexture->activeTexture(1);
            pShaderProgram->setUniformValue("objectMaterial.diffuse", 1);
        }

        // Set Specular Texture
        QSharedPointer<GPUFilterTexture> pSpecularTexture = m_pMaterial->getSpecularTexture();
        if (pSpecularTexture)
        {
            pSpecularTexture->activeTexture(2);
            pShaderProgram->setUniformValue("objectMaterial.specular", 2);
        }
    }

    // process Extra Texture
    bool isEnabledExtraTexture = m_pMaterial->isExtraTextureEnabled();
    pShaderProgram->setUniformValue("objectMaterial.isUsedExtraSample", isEnabledExtraTexture);
    if (isEnabledExtraTexture)
    {
        // Extra Texture1
        QSharedPointer<GPUFilterTexture> pTexture1 = m_pMaterial->getExtraTexture1();
        if (pTexture1)
        {
            pTexture1->activeTexture(3);
            pShaderProgram->setUniformValue("objectMaterial.extraSample1", 3);
        }

        // Extra Texture2
        QSharedPointer<GPUFilterTexture> pTexture2 = m_pMaterial->getExtraTexture2();
        if (pTexture2)
        {
            pTexture2->activeTexture(4);
            pShaderProgram->setUniformValue("objectMaterial.extraSample2", 4);
        }

        // Extra Texture3
        QSharedPointer<GPUFilterTexture> pTexture3 = m_pMaterial->getExtraTexture3();
        if (pTexture3)
        {
            pTexture3->activeTexture(5);
            pShaderProgram->setUniformValue("objectMaterial.extraSample3", 5);
        }
    }

    g_GPUFunc->glBindBuffer(GL_TEXTURE_2D, 0);
}

void GPUFilterMesh::processMaterialTexture(void)
{
    if (m_pMaterial.isNull())
        return;

    // Get Shader Program
    QOpenGLShaderProgram* pShaderProgram = nullptr;
    GPUFilterScene* pScene = getCurrentScene();
    if (pScene)
        pShaderProgram = pScene->getShaderProgram();
    if (pShaderProgram == nullptr)
        return;

    // Set Color Enabled
    bool isEnabledColor = m_pMaterial->getColorEnabled();
    if (!isEnabledColor)
    {
        // Set Texture Material

        // Set Ambient Texture
        QSharedPointer<GPUFilterTexture> pAmbientTexture = m_pMaterial->getAmbientTexture();
        if (pAmbientTexture && !pAmbientTexture->hasCreated())
            pAmbientTexture->create();

        // Set Diffues Texture
        QSharedPointer<GPUFilterTexture> pDiffuseTexture = m_pMaterial->getDiffuesTexture();
        if (pDiffuseTexture && !pDiffuseTexture->hasCreated())
            pDiffuseTexture->create();

        // Set Specular Texture
        QSharedPointer<GPUFilterTexture> pSpecularTexture = m_pMaterial->getSpecularTexture();
        if (pSpecularTexture && !pSpecularTexture->hasCreated())
            pSpecularTexture->create();
    }

    // Set Extra Texture
    if (m_pMaterial->isExtraTextureEnabled())
    {
        // Create Extra Texture
        QSharedPointer<GPUFilterTexture> pTexture1 = m_pMaterial->getExtraTexture1();
        if (pTexture1 && !pTexture1->hasCreated())
            pTexture1->create();

        QSharedPointer<GPUFilterTexture> pTexture2 = m_pMaterial->getExtraTexture2();
        if (pTexture2 && !pTexture2->hasCreated())
            pTexture2->create();

        QSharedPointer<GPUFilterTexture> pTexture3 = m_pMaterial->getExtraTexture3();
        if (pTexture3 && !pTexture3->hasCreated())
            pTexture3->create();
    }
}

void GPUFilterMesh::onAmbientTextureChanged(void)
{
    if (m_pMaterial.isNull())
        return;

    QSharedPointer<GPUFilterTexture> pTexture = m_pMaterial->getAmbientTexture();
    if (pTexture.isNull())
        return;

    if (m_hasCreated && !pTexture->hasCreated())
        pTexture->create();
}

void GPUFilterMesh::onDiffuesTextureChanged(void)
{
    if (m_pMaterial.isNull())
        return;

    QSharedPointer<GPUFilterTexture> pTexture = m_pMaterial->getDiffuesTexture();
    if (pTexture.isNull())
        return;

    if (m_hasCreated && !pTexture->hasCreated())
        pTexture->create();
}

void GPUFilterMesh::onSpecularTextureChanged(void)
{
    if (m_pMaterial.isNull())
        return;

    QSharedPointer<GPUFilterTexture> pTexture = m_pMaterial->getSpecularTexture();
    if (pTexture.isNull())
        return;

    if (m_hasCreated && !pTexture->hasCreated())
        pTexture->create();
}

void GPUFilterMesh::onExtraTexture1Changed(void)
{
    if (m_pMaterial.isNull())
        return;

    QSharedPointer<GPUFilterTexture> pTexture = m_pMaterial->getExtraTexture1();
    if (pTexture.isNull())
        return;

    if (m_hasCreated && !pTexture->hasCreated())
        pTexture->create();
}

void GPUFilterMesh::onExtraTexture2Changed(void)
{
    if (m_pMaterial.isNull())
        return;

    QSharedPointer<GPUFilterTexture> pTexture = m_pMaterial->getExtraTexture2();
    if (pTexture.isNull())
        return;

    if (m_hasCreated && !pTexture->hasCreated())
        pTexture->create();
}

void GPUFilterMesh::onExtraTexture3Changed(void)
{
    if (m_pMaterial.isNull())
        return;

    QSharedPointer<GPUFilterTexture> pTexture = m_pMaterial->getExtraTexture3();
    if (pTexture.isNull())
        return;

    if (m_hasCreated && !pTexture->hasCreated())
        pTexture->create();
}

// Visible
void GPUFilterMesh::setVisible(bool isVisible)
{
    m_isVisible = isVisible;
}

bool GPUFilterMesh::isVisible(void)
{
    return m_isVisible;
}

int GPUFilterMesh::getVertexCount(void)
{
    return m_posVec.size();
}

// Animation
void GPUFilterMesh::setAnimationEnabled(bool isEnabled)
{
    m_isAnimation = isEnabled;
}

bool GPUFilterMesh::isAnimationEnabled(void)
{
    return m_isAnimation;
}

