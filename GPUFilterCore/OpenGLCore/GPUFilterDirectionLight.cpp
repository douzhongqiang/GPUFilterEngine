#include "GPUFilterDirectionLight.h"

GPUFilterDirectionLight::GPUFilterDirectionLight(QObject* parent)
    :GPUFilterLightBase(parent)
{

}

GPUFilterDirectionLight::~GPUFilterDirectionLight()
{

}

void GPUFilterDirectionLight::setDirection(const QVector3D& direction)
{
    m_direction = direction;
}

QVector3D GPUFilterDirectionLight::getDirection(void)
{
    return m_direction;
}

int GPUFilterDirectionLight::getType(void)
{
    return 0;
}

void GPUFilterDirectionLight::processShader(QOpenGLShaderProgram* pShaderProgram, int index)
{
    QString tempString("lightMaterial[%1].%2");

    QString uniformString = tempString.arg(index).arg("lightDirection");
    pShaderProgram->setUniformValue(uniformString.toStdString().c_str(), m_direction);

    GPUFilterLightBase::processShader(pShaderProgram, index);
}
