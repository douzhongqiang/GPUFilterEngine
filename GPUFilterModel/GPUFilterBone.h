#ifndef GPUFILTERBONE_H
#define GPUFILTERBONE_H

#include <QObject>
#include <QMatrix4x4>
#include <QVector3D>

class aiNodeAnim;
class GPUFilterBone : public QObject
{
    Q_OBJECT

public:
    struct KeyPostion
    {
        QVector3D postion;
        float timeStamp;
    };

    struct KeyRotation
    {
        QQuaternion orientation;
        float timeStamp;
    };

    struct KeyScale
    {
        QVector3D scale;
        float timeStamp;
    };

public:
    GPUFilterBone(QObject* parent = nullptr);
    ~GPUFilterBone();

    void init(QString name, const aiNodeAnim* channel);
    void update(float animationTime);

    QMatrix4x4 getLocalTransformMatrix(void);

private:
    QVector<KeyPostion> m_postionKeyVec;
    QVector<KeyRotation> m_rotationKeyVec;
    QVector<KeyScale> m_scaleKeyVec;

    QString m_name;
    QMatrix4x4 m_localTransfrom;

    // interpolate
    QMatrix4x4 interpolatePostion(float animationTime);
    QMatrix4x4 interploateRotation(float animationTime);
    QMatrix4x4 interploateScaling(float animationTime);

    int getPostionIndex(float animationTime);
    int getRotationIndex(float animationTime);
    int getScaleIndex(float animationTime);

    float getScaleFactor(float lastTimeStamp, float nextStamp, float animationTime);
};

#endif
