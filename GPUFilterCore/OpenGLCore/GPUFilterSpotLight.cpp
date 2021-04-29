#include "GPUFilterSpotLight.h"
#include <QtMath>

GPUFilterSpotLight::GPUFilterSpotLight(QObject* parent)
    :GPUFilterLightBase(parent)
    , m_lightPostion(0.0f, 0.0f, 0.0f)
    , m_direction(0.0f, 0.0f, -1.0f)
    , m_cutout(12.5f)
    , m_outerCutoff(17.5f)
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
    QString tempString("lightMaterial[%1].%2");

    QString uniformString = tempString.arg(index).arg("lightDirection");
    pShaderProgram->setUniformValue(uniformString.toStdString().c_str(), m_direction);

    uniformString = tempString.arg(index).arg("lightPoint");
    pShaderProgram->setUniformValue(uniformString.toStdString().c_str(), m_lightPostion);

    uniformString = tempString.arg(index).arg("cutoff");
    pShaderProgram->setUniformValue(uniformString.toStdString().c_str(), (float)qCos(qDegreesToRadians(m_cutout)));

    uniformString = tempString.arg(index).arg("outerCutoff");
    pShaderProgram->setUniformValue(uniformString.toStdString().c_str(), (float)qCos(qDegreesToRadians(m_outerCutoff)));

    GPUFilterLightBase::processShader(pShaderProgram, index);
}
