#ifndef GPUFILTERNODE_H
#define GPUFILTERNODE_H

#include <QObject>
#include "OpenGLCore/GPUFilterMesh.h"

class GPUFilterNode : public QObject
{
    Q_OBJECT

public:
    GPUFilterNode(QObject* parent = nullptr);
    ~GPUFilterNode();

    // Get / Set Node Name
    void setNodeName(const QString& nodeName);
    QString getNodeName(void);

    void setParentNode(GPUFilterNode* pNode);
    GPUFilterNode* getParentNode(void);

    void addChildNode(GPUFilterNode* pNode);
    QVector<GPUFilterNode*> getChilds(void);
    int getChildCount(void);

    // Draw
    void draw(void);

    // Add Mesh
    void addMesh(GPUFilterMesh* pMesh);

private:
    QString m_nodeName;

    GPUFilterNode* m_pParentNode;
    QVector<GPUFilterNode*> m_childNodes;

    QVector<GPUFilterMesh*> m_meshVec;
};

#endif
