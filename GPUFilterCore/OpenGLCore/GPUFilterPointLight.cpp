#include "GPUFilterPointLight.h"

GPUFilterPointLight::GPUFilterPointLight(QObject* parent)
    :GPUFilterLightBase(parent)
    , m_constant(1.0f)
    , m_linear(0.09f)
    , m_quadratic(0.032f)
{

}

GPUFilterPointLight::~GPUFilterPointLight()
{

}

// get/set light postion
void GPUFilterPointLight::setLightPostion(const QVector3D& postionVec)
{
    m_LightPostion = postionVec;
}

QVector3D GPUFilterPointLight::getLightPostion(void)
{
    return m_LightPostion;
}

// get/set attenuation
void GPUFilterPointLight::setAttenuationInfo(float constant, float linear, float quadratic)
{
    m_constant = constant;
    m_linear = linear;
    m_quadratic = quadratic;
}

void GPUFilterPointLight::getAttenuationInfo(float& constant, float& linear, float& quadratic)
{
    constant = m_constant;
    linear = m_linear;
    quadratic = m_quadratic;
}

int GPUFilterPointLight::getType(void)
{
    return 1;
}

void GPUFilterPointLight::processShader(QOpenGLShaderProgram* pShaderProgram, int index)
{
    QString tempString("lightMaterial[%1].%2");

    QString uniformString = tempString.arg(index).arg("lightPoint");
    pShaderProgram->setUniformValue(uniformString.toStdString().c_str(), m_LightPostion);

    uniformString = tempString.arg(index).arg("constant");
    pShaderProgram->setUniformValue(uniformString.toStdString().c_str(), m_constant);

    uniformString = tempString.arg(index).arg("linear");
    pShaderProgram->setUniformValue(uniformString.toStdString().c_str(), m_linear);

    uniformString = tempString.arg(index).arg("quadratic");
    pShaderProgram->setUniformValue(uniformString.toStdString().c_str(), m_quadratic);

    GPUFilterLightBase::processShader(pShaderProgram, index);
}
