#include "GPUFilterModel.h"
#include <QFile>
#include <QFileInfo>
#include <QDebug>
#include <QVector3D>

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
        // 设置点信息
        aiVector3D pVertices = pMesh->mVertices[i];
        QVector3D pointPos;
        pointPos.setX(pVertices.x);
        pointPos.setY(pVertices.y);
        pointPos.setZ(pVertices.z);
        pFilterMesh->addVertexPostion(pointPos);

#if 0
        aiVector3D pNormal = pMesh->mNormals[i];
        attrPoint.normal[0] = pNormal.x;
        attrPoint.normal[1] = pNormal.y;
        attrPoint.normal[2] = pNormal.z;

        if (pMesh->mTextureCoords[0])
        {
            attrPoint.textureCoord[0] = pMesh->mTextureCoords[0][i].x;
            attrPoint.textureCoord[1] = pMesh->mTextureCoords[0][i].y;
        }
        else
        {
            attrPoint.textureCoord[0] = 0.0f;
            attrPoint.textureCoord[1] = 0.0f;
        }

        pointArray.append(attrPoint);
#endif
    }

#if 0
    pOpenglMesh->setPoints(pointArray);

    // 设置索引
    QVector<unsigned int> indicesVec;
    for (unsigned int i =0; i<pMesh->mNumFaces; ++i)
    {
        aiFace face = pMesh->mFaces[i];
        for (unsigned int j=0; j<face.mNumIndices; ++j)
        {
            indicesVec << face.mIndices[j];
        }
    }
    pOpenglMesh->setIndices(indicesVec);

    // 加载材质
    qDebug() << pMesh->mName.data;
    int nMaterialIndex = pMesh->mMaterialIndex;
    if (nMaterialIndex >= 0)
    {
        aiMaterial* pMaterial = pScene->mMaterials[nMaterialIndex];
        processMaterial(pMaterial, aiTextureType_DIFFUSE, pOpenglMesh);
        processMaterial(pMaterial, aiTextureType_SPECULAR, pOpenglMesh);
    }

    pOpenglMesh->setupMesh();
#endif
}

void GPUFilterModel::processMaterial(aiMaterial* pMaterial, aiTextureType type, GPUFilterMesh* pOpenGLMaterial)
{

}

void GPUFilterModel::draw(void)
{

}
