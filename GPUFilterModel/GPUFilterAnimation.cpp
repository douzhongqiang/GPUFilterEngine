#include "GPUFilterAnimation.h"
#include "GPUFilterModel.h"

GPUFilterAnimation::GPUFilterAnimation(QObject* parent)
    :QObject(parent)
{

}

GPUFilterAnimation::~GPUFilterAnimation()
{
    delete m_pRootNode;
}

bool GPUFilterAnimation::loadAnimation(const QString& animationPath, GPUFilterModel* pModel)
{
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(animationPath.toStdString(), aiProcess_Triangulate);
    if (scene == nullptr || scene->mRootNode == nullptr)
        return false;

    aiAnimation* animation = scene->mAnimations[0];
    m_duration = animation->mDuration;
    m_nTicksPerSecond = animation->mTicksPerSecond;

    // Fill Nodes
    m_pRootNode = new AssimpNodeData;
    fillNodeDatas(m_pRootNode, scene->mRootNode);
    // Fill Miss Bond
    fillMissBones(animation, pModel);

    dumpInfo();

    return true;
}

void GPUFilterAnimation::fillNodeDatas(AssimpNodeData* pNode, aiNode* pAiNode)
{
    if (pAiNode == nullptr)
        return;

    pNode->nodeName = QString(pAiNode->mName.data);
    pNode->transformMat = GPUFilterModel::converMatrixToQtFortmat(pAiNode->mTransformation);

    int nChildCount = pAiNode->mNumChildren;
    for (int i=0; i<nChildCount; ++i)
    {
        AssimpNodeData* pChildNodeData = new AssimpNodeData;
        fillNodeDatas(pChildNodeData, pAiNode->mChildren[i]);
        pNode->m_childs.append(pChildNodeData);
    }
}

void GPUFilterAnimation::fillMissBones(aiAnimation* pAnimation, GPUFilterModel* pModel)
{
    QMap<QString, GPUFilterBone::BoneInfo>& boneInfoMaps = pModel->getBoneInfoMap();
    int nBoneCount = pModel->getBoneCount();

    int nSize = pAnimation->mNumChannels;
    for (int i=0; i<nSize; ++i)
    {
        aiNodeAnim* channel = pAnimation->mChannels[i];
        QString boneName = QString(channel->mNodeName.data);

        if (boneInfoMaps.find(boneName) == boneInfoMaps.end())
        {
            GPUFilterBone::BoneInfo newBone;
            newBone.id = nBoneCount++;
            boneInfoMaps[boneName] = newBone;
        }

        GPUFilterBone* pBone = new GPUFilterBone(this);
        pBone->init(boneName, boneInfoMaps[boneName].id, channel);
        m_bones.push_back(pBone);
    }

    m_boneMaps = boneInfoMaps;
}

GPUFilterBone* GPUFilterAnimation::getBone(const QString& name)
{
    for (auto iter = m_bones.begin(); iter != m_bones.end(); ++iter)
    {
        if (name == (*iter)->getName())
            return *iter;
    }

    return nullptr;
}

float GPUFilterAnimation::getDuration(void)
{
    return m_duration;
}

int GPUFilterAnimation::getTicksPerSecond(void)
{
    return m_nTicksPerSecond;
}

QMap<QString, GPUFilterBone::BoneInfo> GPUFilterAnimation::getBondIDMap(void)
{
    return m_boneMaps;
}

GPUFilterAnimation::AssimpNodeData* GPUFilterAnimation::getRootNode(void)
{
    return m_pRootNode;
}

void GPUFilterAnimation::dumpInfo(void)
{
    // Bone Info
    qDebug() << "================================== Bone Info: ==================================";
    QStringList boneNames;
    for (auto iter = m_boneMaps.begin(); iter != m_boneMaps.end(); ++iter)
        boneNames << iter.key();
    qDebug() << boneNames.join(",");
    qDebug() << "================================== Bone Info End ==================================";

    // Bone Channels
    qDebug() << "================================== Channel Names Info: ==================================";
    boneNames.clear();
    for (auto iter = m_bones.begin(); iter != m_bones.end(); ++iter)
        boneNames << (*iter)->getName();
    qDebug() << boneNames.join(",");
    qDebug() << "================================== Channel Names Info End ==================================";

    // Animation Node Info
    qDebug() << "================================== Animation Node Info: ==================================";
    boneNames.clear();
    boneNames = getRootNodeNames(m_pRootNode);
    qDebug() << boneNames.join(",");
    qDebug() << "================================== Animation Node Info End ==================================";
}

QStringList GPUFilterAnimation::getRootNodeNames(AssimpNodeData* pNode)
{
    QStringList strList;
    strList << pNode->nodeName;

    for (auto iter = pNode->m_childs.begin(); iter != pNode->m_childs.end(); ++iter)
    {
        QStringList tempStrList = getRootNodeNames((*iter));
        strList += tempStrList;
    }

    return strList;
}
