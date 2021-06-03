#ifndef GPUFILTERMODEL_H
#define GPUFILTERMODEL_H

#include <QObject>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <QSharedPointer>
#include <QMatrix4x4>
#include "GPUFilterNode.h"
#include "OpenGLCore/GPUFilterMesh.h"
#include "OpenGLCore/GPUFilterTexture.h"
#include "GPUFilterModel_global.h"

class GPUFilterScene;
class GPUFILTERMODEL_EXPORT GPUFilterModel : public QObject
{
    Q_OBJECT

public:
    GPUFilterModel(QObject* parent = nullptr);
    ~GPUFilterModel();

    bool loadModel(const QString& path);
    void processNode(aiNode* pAiNode, const aiScene* pScene, GPUFilterNode* pNode);
    void processMesh(aiMesh* pMesh, const aiScene* pScene, GPUFilterNode* pNode);
    void processMaterial(aiMaterial* pMaterial, aiTextureType type, GPUFilterMaterial* pFilterMaterial);

    void draw(void);

    void addToScene(GPUFilterScene* pScene);
    void addToScene(GPUFilterNode* pNode, GPUFilterScene* pScene);

    void setModelMatrix(const QMatrix4x4& mat);
    QMatrix4x4 getModelMatrix(void);

private:
    QString m_dirPath;
    GPUFilterNode* m_pRootNode = nullptr;

    QVector<QSharedPointer<GPUFilterTexture>> m_textures;
    QMatrix4x4 m_modelMatrix;
};
#endif
