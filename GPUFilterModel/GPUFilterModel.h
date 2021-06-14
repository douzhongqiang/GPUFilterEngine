#ifndef GPUFILTERMODEL_H
#define GPUFILTERMODEL_H

#include <QObject>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <QSharedPointer>
#include <QMatrix4x4>
#include <QTime>
#include "GPUFilterNode.h"
#include "OpenGLCore/GPUFilterMesh.h"
#include "OpenGLCore/GPUFilterTexture.h"
#include "GPUFilterModel_global.h"
#include "GPUFilterBone.h"

class GPUFilterScene;
class GPUFilterBoneMesh;
class GPUFilterAnimation;
class GPUFilterAnimator;
class GPUFILTERMODEL_EXPORT GPUFilterModel : public QObject
{
    Q_OBJECT

public:
    GPUFilterModel(QObject* parent = nullptr);
    ~GPUFilterModel();

    bool loadModel(const QString& path, bool isLoadAnimation = false);
    bool loadAnimation(const QString& path);

    void processNode(aiNode* pAiNode, const aiScene* pScene, GPUFilterNode* pNode);
    void processMesh(aiMesh* pMesh, const aiScene* pScene, GPUFilterNode* pNode);
    void processMaterial(aiMaterial* pMaterial, aiTextureType type, GPUFilterMaterial* pFilterMaterial);

    void draw(void);

    void addToScene(GPUFilterScene* pScene);
    void addToScene(GPUFilterNode* pNode, GPUFilterScene* pScene);

    void setModelMatrix(const QMatrix4x4& mat);
    QMatrix4x4 getModelMatrix(void);

    static QMatrix4x4 converMatrixToQtFortmat(const aiMatrix4x4& from);

    // Bone About
    QMap<QString, GPUFilterBone::BoneInfo>& getBoneInfoMap(void);
    int getBoneCount(void);

    bool isAnimationEnabled(void);

private:
    bool m_isLoadAnimation = false;
    QString m_dirPath;
    GPUFilterNode* m_pRootNode = nullptr;
    GPUFilterScene* m_pScene = nullptr;

    QVector<QSharedPointer<GPUFilterTexture>> m_textures;
    QMatrix4x4 m_modelMatrix;

    QTime m_time;
    bool m_isLoaded = false;

    // ========================================================================
private:
    // Bone About
    QMap<QString, GPUFilterBone::BoneInfo> m_boneMaps;
    int m_nBoneCount = 0;

    void extractBoneWeightForVertices(aiMesh* mesh, const aiScene* scene, GPUFilterBoneMesh* pFilterMesh);

private:
    // Animation About
    GPUFilterAnimation* m_pAnimation = nullptr;
    GPUFilterAnimator* m_pAnimator = nullptr;

    void initAnimation(void);
};
#endif
