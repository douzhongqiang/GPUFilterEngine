#include "GPUFilterModel.h"
#include <QFile>
#include <QFileInfo>
#include <QDebug>
#include <QVector3D>
#include "OpenGLCore/GPUFilterMaterial.h"
#include "OpenGLCore/GPUFilterScene.h"
#include "3DExtras/GPUFilterGeometryCubeBox.h"

GPUFilterModel::GPUFilterModel(QObject* parent)
    :QObject(parent)
{

}

GPUFilterModel::~GPUFilterModel()
{

}

bool GPUFilterModel::loadModel(const QString& path)
{
    QFile file(path);
    if (!file.exists())
        return false;

    QFileInfo info(path);
    m_dirPath = info.absolutePath();

    // 加载模型
    Assimp::Importer importer;
    const aiScene* pScene = importer.ReadFile(path.toStdString().c_str(), aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenNormals);
    if (pScene == nullptr || pScene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !pScene->mRootNode)
    {
        qDebug() << "Load Model Error" << importer.GetErrorString();
        return false;
    }

    m_pRootNode = new GPUFilterNode;
    m_pRootNode->setNodeName(QString(pScene->mRootNode->mName.data));
    this->processNode(pScene->mRootNode, pScene, m_pRootNode);
    return true;
}

void GPUFilterModel::processNode(aiNode* pAiNode, const aiScene* pScene, GPUFilterNode* pNode)
{
    for (unsigned int i=0; i<pAiNode->mNumMeshes; ++i)
    {
        aiMesh* pMesh = pScene->mMeshes[pAiNode->mMeshes[i]];
        processMesh(pMesh, pScene, pNode);
    }

    for (unsigned int i=0; i<pAiNode->mNumChildren; ++i)
    {
        aiNode* childNode = pAiNode->mChildren[i];

        GPUFilterNode* pChildFilterNode = new GPUFilterNode;
        pChildFilterNode->setNodeName(childNode->mName.data);
        pNode->addChildNode(pChildFilterNode);

        processNode(childNode, pScene, pChildFilterNode);
    }
}

void GPUFilterModel::processMesh(aiMesh* pMesh, const aiScene* pScene, GPUFilterNode* pNode)
{
    GPUFilterMesh* pFilterMesh = new GPUFilterMesh(pNode);
    pNode->addMesh(pFilterMesh);

    for (unsigned int i=0; i<pMesh->mNumVertices; ++i)
    {
        // Add Vertices
        aiVector3D pVertices = pMesh->mVertices[i];
        QVector3D pointPos;
        pointPos.setX(pVertices.x);
        pointPos.setY(pVertices.y);
        pointPos.setZ(pVertices.z);
        pFilterMesh->addVertexPostion(pointPos);

        // Add Normal
        aiVector3D pNormal = pMesh->mNormals[i];
        QVector3D normalVec;
        normalVec.setX(pNormal.x);
        normalVec.setY(pNormal.y);
        normalVec.setZ(pNormal.z);
        pFilterMesh->addNormal(normalVec);

        // Add Texture Coord
        QVector3D textureCoord;
        if (pMesh->mTextureCoords[0])
        {
            textureCoord.setX(pMesh->mTextureCoords[0][i].x);
            textureCoord.setY(pMesh->mTextureCoords[0][i].y);
        }

        pFilterMesh->addCoord(textureCoord);
    }

    // Set indices
    QVector<unsigned int> indicesVec;
    for (unsigned int i =0; i<pMesh->mNumFaces; ++i)
    {
        aiFace face = pMesh->mFaces[i];
        for (unsigned int j=0; j<face.mNumIndices; ++j)
        {
            indicesVec << face.mIndices[j];
        }
    }
    pFilterMesh->setIndices(indicesVec);

    // Set Material
    int nMaterialIndex = pMesh->mMaterialIndex;
    if (nMaterialIndex >= 0)
    {
        aiMaterial* pMaterial = pScene->mMaterials[nMaterialIndex];
        GPUFilterMaterial* pFilterMaterial = new GPUFilterMaterial(pFilterMesh);
        pFilterMesh->setMaterial(QSharedPointer<GPUFilterMaterial>(pFilterMaterial));
        pFilterMaterial->setColorEnabled(true);
        pFilterMaterial->setAmbientColor(QVector3D(0.2f, 0.2f, 0.2f));
        pFilterMaterial->setDiffuesColor(QVector3D(0.4f, 0.4f, 0.4f));
        pFilterMaterial->setSpecularColor(QVector3D(0.8f, 0.8f, 0.8f));

        processMaterial(pMaterial, aiTextureType_DIFFUSE, pFilterMaterial);
        processMaterial(pMaterial, aiTextureType_SPECULAR, pFilterMaterial);

    }
}

void GPUFilterModel::processMaterial(aiMaterial* pMaterial, aiTextureType type, GPUFilterMaterial* pFilterMaterial)
{
    unsigned int diffuseCount = pMaterial->GetTextureCount(type);
    for (unsigned int i=0; i<diffuseCount; ++i)
    {
        aiString textureString;
        pMaterial->GetTexture(type, i, &textureString);

        QString texturePath = m_dirPath + "/" + QString(textureString.C_Str());
        qDebug() << type << texturePath;

        bool needCreate = true;
        for (int i=0; i<m_textures.size(); ++i)
        {
            if (texturePath == m_textures[i]->objectName())
            {
                needCreate = false;
                if (type == aiTextureType_DIFFUSE)
                {
                    pFilterMaterial->setAmbientTexture(m_textures[i]);
                    pFilterMaterial->setDiffuesTexture(m_textures[i]);
                }
                else
                    pFilterMaterial->setSpecularTexture(m_textures[i]);
                break;
            }
        }

        if (!needCreate)
            continue;

        QSharedPointer<GPUFilterTexture> pTexture(new GPUFilterTexture(this));
        pTexture->setImage(texturePath);
        m_textures.push_back(pTexture);

        if (type == aiTextureType_DIFFUSE)
        {
            pFilterMaterial->setAmbientTexture(pTexture);
            pFilterMaterial->setDiffuesTexture(pTexture);

            pFilterMaterial->setColorEnabled(false);
            pFilterMaterial->setExtraTextureEnabled(false);
        }
        else
            pFilterMaterial->setSpecularTexture(pTexture);
    }
}

void GPUFilterModel::draw(void)
{
    if (m_pRootNode)
        m_pRootNode->draw();
}

void GPUFilterModel::addToScene(GPUFilterScene* pScene)
{
    if (!m_pRootNode)
        return;

    addToScene(m_pRootNode, pScene);
}

void GPUFilterModel::addToScene(GPUFilterNode* pNode, GPUFilterScene* pScene)
{
    if (!pNode)
        return;

    qDebug() << "Node Name: " << pNode->getNodeName();
    QVector<GPUFilterMesh*> meshVec = pNode->getMesh();
    for (auto iter = meshVec.begin(); iter != meshVec.end(); ++iter)
    {
        GPUFilterMesh* pMesh = *iter;
        pMesh->setModelMartix(m_modelMatrix);

        pScene->addMesh(pMesh);
    }

    QVector<GPUFilterNode*> nodes = pNode->getChilds();
    for (auto iter = nodes.begin(); iter != nodes.end(); ++iter)
    {
        addToScene(*iter, pScene);
    }
}

void GPUFilterModel::setModelMatrix(const QMatrix4x4& mat)
{
    m_modelMatrix = mat;
}

QMatrix4x4 GPUFilterModel::getModelMatrix(void)
{
    return m_modelMatrix;
}
