#ifndef GPUFILTERANIMATION_H
#define GPUFILTERANIMATION_H

#include <QObject>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <QMatrix4x4>
#include "GPUFilterBone.h"

class GPUFilterModel;
class GPUFilterAnimation : public QObject
{
    Q_OBJECT

public:
    struct AssimpNodeData
    {
        QString nodeName;
        QMatrix4x4 transformMat;

        QVector<AssimpNodeData*> m_childs;

        AssimpNodeData(){}
        ~AssimpNodeData(){
            for (auto iter = m_childs.begin(); iter != m_childs.end(); ++iter)
                delete *iter;
        }
    };

public:
    GPUFilterAnimation(QObject* parent = nullptr);
    ~GPUFilterAnimation();

    bool loadAnimation(const QString& animationPath, GPUFilterModel* pModel);
    GPUFilterBone* getBone(const QString& name);

    float getDuration(void);
    int getTicksPerSecond(void);

    QMap<QString, GPUFilterBone::BoneInfo> getBondIDMap(void);
    AssimpNodeData* getRootNode(void);

    void dumpInfo(void);
    QStringList getRootNodeNames(AssimpNodeData* pNode);

private:
    float m_duration = 0.0f;
    int m_nTicksPerSecond = 0;

    AssimpNodeData* m_pRootNode = nullptr;
    void fillNodeDatas(AssimpNodeData* pNode, aiNode* pAiNode);
    void fillMissBones(aiAnimation* pAnimation, GPUFilterModel* pModel);

    QMap<QString, GPUFilterBone::BoneInfo> m_boneMaps;
    QVector<GPUFilterBone*> m_bones;
};

#endif
