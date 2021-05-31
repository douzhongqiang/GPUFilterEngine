#include "GPUFilterNode.h"

GPUFilterNode::GPUFilterNode(QObject* parent)
    :QObject(parent)
{

}

GPUFilterNode::~GPUFilterNode()
{
    for (auto iter = m_childNodes.begin(); iter != m_childNodes.end(); ++iter)
    {
        (*iter)->deleteLater();
    }

    m_childNodes.clear();
}

// Get / Set Node Name
void GPUFilterNode::setNodeName(const QString& nodeName)
{
    m_nodeName = nodeName;
}

QString GPUFilterNode::getNodeName(void)
{
    return m_nodeName;
}

void GPUFilterNode::setParentNode(GPUFilterNode* pNode)
{
    m_pParentNode = pNode;
}

GPUFilterNode* GPUFilterNode::getParentNode(void)
{
    return m_pParentNode;
}

void GPUFilterNode::addChildNode(GPUFilterNode* pNode)
{
    m_childNodes.push_back(pNode);
}

QVector<GPUFilterNode*> GPUFilterNode::getChilds(void)
{
    return m_childNodes;
}

int GPUFilterNode::getChildCount(void)
{
    return m_childNodes.size();
}

void GPUFilterNode::draw(void)
{
    // Draw Self
    for (auto iter = m_meshVec.begin(); iter != m_meshVec.end(); ++iter)
    {
        GPUFilterMesh* pMesh = *iter;
        pMesh->draw();
    }

    // Draw Sub Nodes
    for (auto iter = m_childNodes.begin(); iter != m_childNodes.end(); ++iter)
    {
        GPUFilterNode* pNode = *iter;
        pNode->draw();
    }
}

void GPUFilterNode::addMesh(GPUFilterMesh* pMesh)
{
    m_meshVec.push_back(pMesh);
}
