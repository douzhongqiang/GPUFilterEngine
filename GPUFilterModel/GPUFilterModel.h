#ifndef GPUFILTERMODEL_H
#define GPUFILTERMODEL_H

#include <QObject>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "GPUFilterNode.h"
#include "OpenGLCore/GPUFilterMesh.h"

class GPUFilterModel : public QObject
{
    Q_OBJECT

public:
    GPUFilterModel(QObject* parent = nullptr);
    ~GPUFilterModel();

    bool loadModel(const QString& path);
    void processNode(aiNode* pAiNode, const aiScene* pScene, GPUFilterNode* pNode);
    void processMesh(aiMesh* pMesh, const aiScene* pScene, GPUFilterNode* pNode);
    void processMaterial(aiMaterial* pMaterial, aiTextureType type, GPUFilterMesh* pOpenGLMaterial);

    void draw(void);

private:
    QString m_dirPath;
    GPUFilterNode* m_pRootNode = nullptr;
};
#endif
