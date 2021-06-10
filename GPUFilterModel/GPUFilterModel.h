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
class GPUFilterBoneMesh;
class GPUFILTERMODEL_EXPORT GPUFilterModel : public QObject
{
    Q_OBJECT
public:
    struct BoneInfo
    {
        int id;
        QMatrix4x4 offsetMatrix;
        QString boneName;
    };

public:
    GPUFilterModel(QObject* parent = nullptr);
    ~GPUFilterModel();

    bool loadModel(const QString& path, bool isLoadAnimation = false);
    void processNode(aiNode* pAiNode, const aiScene* pScene, GPUFilterNode* pNode);
    void processMesh(aiMesh* pMesh, const aiScene* pScene, GPUFilterNode* pNode);
    void processMaterial(aiMaterial* pMaterial, aiTextureType type, GPUFilterMaterial* pFilterMaterial);

    void draw(void);

    void addToScene(GPUFilterScene* pScene);
    void addToScene(GPUFilterNode* pNode, GPUFilterScene* pScene);

    void setModelMatrix(const QMatrix4x4& mat);
    QMatrix4x4 getModelMatrix(void);

private:
    bool m_isLoadAnimation = false;
    QString m_dirPath;
    GPUFilterNode* m_pRootNode = nullptr;

    QVector<QSharedPointer<GPUFilterTexture>> m_textures;
    QMatrix4x4 m_modelMatrix;

    // ========================================================================
private:
    // Bone About
    QMap<QString, BoneInfo> m_boneMaps;
    int m_nBoneCount = 0;

    void extractBoneWeightForVertices(aiMesh* mesh, const aiScene* scene, GPUFilterBoneMesh* pFilterMesh);
    QMatrix4x4 converMatrixToQtFortmat(const aiMatrix4x4& from);
};
#endif
