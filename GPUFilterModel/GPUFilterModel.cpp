#include "GPUFilterModel.h"
#include <QFile>
#include <QFileInfo>
#include <QDebug>
#include <QVector3D>
#include <QOpenGLShaderProgram>
#include "OpenGLCore/GPUFilterMaterial.h"
#include "OpenGLCore/GPUFilterScene.h"
#include "3DExtras/GPUFilterGeometryCubeBox.h"
#include "GPUFilterBoneMesh.h"
#include "GPUFilterModel.h"
#include "GPUFilterAnimation.h"
#include "GPUFilterAnimator.h"
#include "OpenGLCore/GPUFilterShaderProgram.h"

GPUFilterModel::GPUFilterModel(QObject* parent)
    :QObject(parent)
{

}

GPUFilterModel::~GPUFilterModel()
{

}

bool GPUFilterModel::loadModel(const QString& path, bool isLoadAnimation)
{
    QFile file(path);
    if (!file.exists())
        return false;

    m_isLoadAnimation = isLoadAnimation;
    QFileInfo info(path);
    m_dirPath = info.absolutePath();

    // Load Model
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

bool GPUFilterModel::loadAnimation(const QString& path)
{
    // Load Animation
    bool result = true;

    if (m_isLoadAnimation)
    {
        initAnimation();
        result = m_pAnimation->loadAnimation(path, this);
        m_pAnimator->setCurrentAnimation(m_pAnimation);
    }

    m_isLoaded = true;
    return result;
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
    GPUFilterMesh* pFilterMesh = nullptr;
    if (!m_isLoadAnimation)
        pFilterMesh = new GPUFilterMesh(pNode);
    else
        pFilterMesh = new GPUFilterBoneMesh(pNode);
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

        // Add Bone Infos
        GPUFilterBoneMesh* pBoneMesh = dynamic_cast<GPUFilterBoneMesh*>(pFilterMesh);
        if (pBoneMesh)
            pBoneMesh->addOneDefBoneInfo();
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

    // Init Bone
    GPUFilterBoneMesh* pBoneMesh = dynamic_cast<GPUFilterBoneMesh*>(pFilterMesh);
    if (pBoneMesh)
        extractBoneWeightForVertices(pMesh, pScene, pBoneMesh);
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
    if (m_pScene && m_isLoaded && m_isLoadAnimation)
    {
        static bool isFirstRender = true;
        if (isFirstRender)
        {
            isFirstRender = false;
            m_time.start();
        }
        else
        {
            int timeInterval = m_time.elapsed();
            m_pAnimator->update(timeInterval / 1000.0);
            m_time.restart();
        }

        QOpenGLShaderProgram* pShaderProgram = m_pScene->getShaderProgram();
        pShaderProgram->setUniformValue("M_isAnimation", m_isLoadAnimation);
        QVector<QMatrix4x4>& mats = m_pAnimator->getFinalBondVec();
        int nSize = m_pAnimator->getFinaBondSize();

        for (int i=0; i<nSize; ++i)
        {
            QString matKey = "finalBonesMatrices[%1]";
            matKey = matKey.arg(i);
            pShaderProgram->setUniformValue(matKey.toStdString().c_str(), mats[i]);
        }
    }
}

void GPUFilterModel::addToScene(GPUFilterScene* pScene)
{
    if (!m_pRootNode)
        return;

    m_pScene = pScene;
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
        pMesh->setAnimationEnabled(m_isLoadAnimation);

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

void GPUFilterModel::extractBoneWeightForVertices(aiMesh* mesh, const aiScene* scene, GPUFilterBoneMesh* pFilterMesh)
{
    for (int boneIndex = 0; boneIndex < mesh->mNumBones; ++boneIndex)
    {
        int boneID = -1;
        aiBone* pBone = mesh->mBones[boneIndex];
        QString boneName = pBone->mName.C_Str();

        // get bone id
        if (m_boneMaps.find(boneName) == m_boneMaps.end())
        {
            GPUFilterBone::BoneInfo newBoneInfo;
            newBoneInfo.id = m_nBoneCount;
            newBoneInfo.boneName = boneName;
            newBoneInfo.offsetMatrix = converMatrixToQtFortmat(pBone->mOffsetMatrix);

            m_boneMaps.insert(boneName, newBoneInfo);
            boneID = m_nBoneCount++;
        }
        else
            boneID = m_boneMaps[boneName].id;
        assert(boneID != -1);

        // set weight
        aiVertexWeight* pWeight = pBone->mWeights;
        int nWeightNumber = pBone->mNumWeights;

        for (int weightIndex = 0; weightIndex < nWeightNumber; ++weightIndex)
        {
            int vertexId = pWeight[weightIndex].mVertexId;
            float weight = pWeight[weightIndex].mWeight;

            pFilterMesh->updateBoneInfo(vertexId, boneID, weight);
        }
    }
}

QMatrix4x4 GPUFilterModel::converMatrixToQtFortmat(const aiMatrix4x4& from)
{
    QMatrix4x4 to(from.a1, from.b1, from.c1, from.d1, \
                  from.a2, from.b2, from.c2, from.d2, \
                  from.a3, from.b3, from.c3, from.d3, \
                  from.a4, from.b4, from.c4, from.d4);

    to = to.transposed();

    return to;
}

// Bone About
QMap<QString, GPUFilterBone::BoneInfo>& GPUFilterModel::getBoneInfoMap(void)
{
    return m_boneMaps;
}

int GPUFilterModel::getBoneCount(void)
{
    return m_nBoneCount;
}

bool GPUFilterModel::isAnimationEnabled(void)
{
    return m_isLoadAnimation;
}

void GPUFilterModel::initAnimation(void)
{
    m_pAnimation = new GPUFilterAnimation(this);
    m_pAnimator = new GPUFilterAnimator(this);
}
