#ifndef GPUFILTERNODE_H
#define GPUFILTERNODE_H

#include <QObject>
#include "OpenGLCore/GPUFilterMesh.h"
#include "GPUFilterModel_global.h"

class GPUFILTERMODEL_EXPORT GPUFilterNode : public QObject
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
    // get Meshs
    QVector<GPUFilterMesh*> getMesh(void);

private:
    QString m_nodeName;

    GPUFilterNode* m_pParentNode;
    QVector<GPUFilterNode*> m_childNodes;

    QVector<GPUFilterMesh*> m_meshVec;
};

#endif
