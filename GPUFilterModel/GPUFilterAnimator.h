#ifndef GPUFILTERANIMATOR_H
#define GPUFILTERANIMATOR_H

#include <QObject>
#include <QVector>
#include <QMatrix4x4>
#include "GPUFilterAnimation.h"

class GPUFilterAnimator : public QObject
{
    Q_OBJECT

public:
    GPUFilterAnimator(QObject* parent = nullptr);
    ~GPUFilterAnimator();

    void setCurrentAnimation(GPUFilterAnimation* pAnimation);
    void update(float dt);

    QVector<QMatrix4x4>& getFinalBondVec(void);
    int getFinaBondSize(void);

private:
    GPUFilterAnimation* m_pCurrentAnimation = nullptr;
    float m_currentTime = 0.0f;
    float m_deltaTime = 0.0f;
    QVector<QMatrix4x4> m_finalBoneMatrix;

    void calcBoneTransform(GPUFilterAnimation::AssimpNodeData* pAssimpNodeData, QMatrix4x4 parentTransform);
};

#endif
