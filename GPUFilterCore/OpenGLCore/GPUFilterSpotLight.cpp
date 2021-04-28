#include "GPUFilterSpotLight.h"

GPUFilterSpotLight::GPUFilterSpotLight(QObject* parent)
    :GPUFilterLightBase(parent)
{

}

GPUFilterSpotLight::~GPUFilterSpotLight()
{

}

// set/get light postion
void GPUFilterSpotLight::setLightPostion(const QVector3D& postionVec)
{
    m_lightPostion = postionVec;
}

QVector3D GPUFilterSpotLight::getLightPostion(void)
{
    return m_lightPostion;
}

// set/get direction
void GPUFilterSpotLight::setDirection(const QVector3D& direction)
{
    m_direction = direction;
}

QVector3D GPUFilterSpotLight::getDirection(void)
{
    return m_direction;
}

// set/get cutout infos
void GPUFilterSpotLight::setCutoutInfo(float cutout, float cutoff)
{
    m_cutout = cutout;
    m_outerCutoff = cutoff;
}

void GPUFilterSpotLight::getCutoutInfo(float& cutout, float& cutoff)
{
    cutout = m_cutout;
    m_outerCutoff = cutoff;
}

int GPUFilterSpotLight::getType(void)
{
    return 2;
}

void GPUFilterSpotLight::processShader(QOpenGLShaderProgram* pShaderProgram, int index)
{

}
