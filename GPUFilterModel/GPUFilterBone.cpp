#include "GPUFilterBone.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "OpenGLCore/GPUFilterTool.h"

GPUFilterBone::GPUFilterBone(QObject* parent)
    :QObject(parent)
{

}

GPUFilterBone::~GPUFilterBone()
{

}

void GPUFilterBone::init(QString name, int id, aiNodeAnim* channel)
{
    m_name = name;
    m_id = id;

    // Add Key Postion
    int nPostionCount = channel->mNumPositionKeys;
    for (int i=0; i<nPostionCount; ++i)
    {
        aiVector3D aiPostion = channel->mPositionKeys[i].mValue;
        float timeStamp = channel->mPositionKeys[i].mTime;

        KeyPostion data;
        data.postion = QVector3D(aiPostion.x, aiPostion.y, aiPostion.z);
        data.timeStamp = timeStamp;
        m_postionKeyVec << data;
    }

    // Add Key Rotation
    int nRotationCount = channel->mNumRotationKeys;
    for (int i=0; i<nRotationCount; ++i)
    {
        aiQuaternion aiOrientation = channel->mRotationKeys[i].mValue;
        float timeStamp = channel->mRotationKeys[i].mTime;

        KeyRotation data;
        data.orientation = QQuaternion(aiOrientation.w, aiOrientation.x, aiOrientation.y, aiOrientation.z);
        data.timeStamp = timeStamp;
        m_rotationKeyVec << data;
    }

    // Add Key Scale
    int nScaleCout = channel->mNumScalingKeys;
    for (int i=0; i<nScaleCout; ++i)
    {
        aiVector3D aiScale = channel->mScalingKeys[i].mValue;
        float timeStamp = channel->mScalingKeys[i].mTime;

        KeyScale data;
        data.scale = QVector3D(aiScale.x, aiScale.y, aiScale.z);
        data.timeStamp = timeStamp;
    }
}

void GPUFilterBone::update(float animationTime)
{
    QMatrix4x4 translation = interpolatePostion(animationTime);
    QMatrix4x4 rotation = interploateRotation(animationTime);
    QMatrix4x4 scale = interploateScaling(animationTime);

    m_localTransfrom = translation * rotation * scale;
}

QMatrix4x4 GPUFilterBone::getLocalTransformMatrix(void)
{
    return m_localTransfrom;
}

QString GPUFilterBone::getName(void)
{
    return m_name;
}

int GPUFilterBone::getPostionIndex(float animationTime)
{
    for (int index = 0; index<m_postionKeyVec.size() - 1; ++index)
    {
        if (animationTime < m_postionKeyVec[index + 1].timeStamp)
            return index;
    }

    return m_postionKeyVec.size() - 1;
}

int GPUFilterBone::getRotationIndex(float animationTime)
{
    for (int index = 0; index<m_rotationKeyVec.size() - 1; ++index)
    {
        if (animationTime < m_rotationKeyVec[index + 1].timeStamp)
            return index;
    }

    return m_rotationKeyVec.size() - 1;
}

int GPUFilterBone::getScaleIndex(float animationTime)
{
    for (int index = 0; index<m_scaleKeyVec.size() - 1; ++index)
    {
        if (animationTime < m_scaleKeyVec[index + 1].timeStamp)
            return index;
    }

    return m_scaleKeyVec.size() - 1;
}

float GPUFilterBone::getScaleFactor(float lastTimeStamp, float nextStamp, float animationTime)
{
    float scaler = 0.0f;

    float value1 = animationTime - lastTimeStamp;
    float value2 = nextStamp - lastTimeStamp;
    scaler = value1 / value2;

    return scaler;
}

QMatrix4x4 GPUFilterBone::interpolatePostion(float animationTime)
{
    QMatrix4x4 mat;
    if (m_postionKeyVec.size() == 1)
        mat.translate(m_postionKeyVec[0].postion);
    else if (m_postionKeyVec.size() > 0)
    {
        int p0Index = getPostionIndex(animationTime);
        int p1Index = p0Index + 1 >= m_postionKeyVec.size() ? m_postionKeyVec.size() - 1 : p0Index + 1;
        float scaler = getScaleFactor(m_postionKeyVec[p0Index].timeStamp, \
                                      m_postionKeyVec[p1Index].timeStamp, animationTime);

        QVector3D finalPostion = g_GPUFilterTool->mix(m_postionKeyVec[p0Index].postion, \
                                                      m_postionKeyVec[p1Index].postion, scaler);
        mat.translate(finalPostion);
    }

    return mat;
}

QMatrix4x4 GPUFilterBone::interploateRotation(float animationTime)
{
    QMatrix4x4 mat;

    if (m_rotationKeyVec.size() == 1)
        mat.rotate(m_rotationKeyVec[0].orientation);
    else if (m_rotationKeyVec.size() > 0)
    {
        int p0Index = getRotationIndex(animationTime);
        int p1Index = p0Index + 1 >= m_rotationKeyVec.size() ? m_rotationKeyVec.size() - 1 : p0Index + 1;
        float scaler = getScaleFactor(m_rotationKeyVec[p0Index].timeStamp, \
                                      m_rotationKeyVec[p1Index].timeStamp, animationTime);

        QQuaternion finalQuaternion = QQuaternion::slerp(m_rotationKeyVec[p0Index].orientation, \
                                                         m_rotationKeyVec[p1Index].orientation, scaler);
        finalQuaternion.normalized();
        mat.rotate(finalQuaternion);
    }

    return mat;
}

QMatrix4x4 GPUFilterBone::interploateScaling(float animationTime)
{
    QMatrix4x4 mat;
    if (m_scaleKeyVec.size() == 1)
        mat.translate(m_scaleKeyVec[0].scale);
    else if (m_scaleKeyVec.size() > 0)
    {
        int p0Index = getScaleIndex(animationTime);
        int p1Index = p0Index + 1 >= m_scaleKeyVec.size() ? m_scaleKeyVec.size() - 1 : p0Index + 1;
        float scaler = getScaleFactor(m_scaleKeyVec[p0Index].timeStamp, \
                                      m_scaleKeyVec[p1Index].timeStamp, animationTime);

        QVector3D finalPostion = g_GPUFilterTool->mix(m_scaleKeyVec[p0Index].scale, \
                                                      m_scaleKeyVec[p1Index].scale, scaler);
        mat.scale(finalPostion);
    }

    return mat;
}
