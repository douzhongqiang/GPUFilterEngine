#include "GPUFilterAnimator.h"
#include <QDebug>

GPUFilterAnimator::GPUFilterAnimator(QObject* parent)
    :QObject(parent)
{
    //m_finalBoneMatrix.reserve(100);
    for (int i=0; i<100; ++i)
        m_finalBoneMatrix.push_back(QMatrix4x4());

    qDebug() << m_finalBoneMatrix.size();
}

GPUFilterAnimator::~GPUFilterAnimator()
{

}

void GPUFilterAnimator::setCurrentAnimation(GPUFilterAnimation* pAnimation)
{
    m_pCurrentAnimation = pAnimation;
    m_currentTime = 0.0f;
}

void GPUFilterAnimator::update(float dt)
{
    m_deltaTime = dt;

    if (m_pCurrentAnimation)
    {
        m_currentTime += m_pCurrentAnimation->getTicksPerSecond() * dt;
        m_currentTime = fmod(m_currentTime, m_pCurrentAnimation->getDuration());
        calcBoneTransform(m_pCurrentAnimation->getRootNode(), QMatrix4x4());
    }
}

QVector<QMatrix4x4>& GPUFilterAnimator::getFinalBondVec(void)
{
    return m_finalBoneMatrix;
}

int GPUFilterAnimator::getFinaBondSize(void)
{
    return m_finalBoneMatrix.count();
}

void GPUFilterAnimator::calcBoneTransform(GPUFilterAnimation::AssimpNodeData* pAssimpNodeData, QMatrix4x4 parentTransform)
{
    QString nodeName = pAssimpNodeData->nodeName;
    QMatrix4x4 nodeTransform = pAssimpNodeData->transformMat;

    GPUFilterBone* pBone = m_pCurrentAnimation->getBone(nodeName);
    if (pBone)
    {
        pBone->update(m_currentTime);
        nodeTransform = pBone->getLocalTransformMatrix();
    }

    QMatrix4x4 globalTransform = parentTransform * nodeTransform;

    QMap<QString, GPUFilterBone::BoneInfo> bondInfoMap = m_pCurrentAnimation->getBondIDMap();
    if (bondInfoMap.find(nodeName) != bondInfoMap.end())
    {
        int index = bondInfoMap[nodeName].id;
        QMatrix4x4 offset = bondInfoMap[nodeName].offsetMatrix;
        m_finalBoneMatrix[index] = globalTransform * offset;
    }

    for (int i = 0; i<pAssimpNodeData->m_childs.count(); ++i)
    {
        GPUFilterAnimation::AssimpNodeData* pSubNoe = pAssimpNodeData->m_childs[i];
        calcBoneTransform(pSubNoe, globalTransform);
    }
}
