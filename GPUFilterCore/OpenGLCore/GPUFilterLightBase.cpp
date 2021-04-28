#include "GPUFilterLightBase.h"

GPUFilterLightBase::GPUFilterLightBase(QObject* parent)
    :QObject(parent)
    , m_ambientColor(0.2f, 0.2f, 0.2f)
    , m_diffuesColor(0.5f, 0.5f, 0.5f)
    , m_specularColor(1.0f, 1.0f, 1.0f)
{

}

GPUFilterLightBase::~GPUFilterLightBase()
{

}

// set/get Ambient
void GPUFilterLightBase::setAmbientColor(const QVector3D& color)
{
    m_ambientColor = color;
}

QVector3D GPUFilterLightBase::getAmbientColor(void)
{
    return m_ambientColor;
}

// set/get Diffuse
void GPUFilterLightBase::setDiffuesColor(const QVector3D& color)
{
    m_diffuesColor = color;
}

QVector3D GPUFilterLightBase::getDiffuesColor(void)
{
    return m_diffuesColor;
}

// set/get Specular
void GPUFilterLightBase::setSpecularColor(const QVector3D& color)
{
    m_specularColor = color;
}

QVector3D GPUFilterLightBase::getSpecularColor(void)
{
    return m_specularColor;
}

int GPUFilterLightBase::getType(void)
{
    return m_type;
}

void GPUFilterLightBase::processShader(QOpenGLShaderProgram* pShaderProgram, int index)
{
    QString tempString("lightMaterial[%1].%2");

    QString uniformString = tempString.arg(index).arg("type");
    pShaderProgram->setUniformValue(uniformString.toStdString().c_str(), this->getType());

    uniformString = tempString.arg(index).arg("ambientColor");
    pShaderProgram->setUniformValue(uniformString.toStdString().c_str(), m_ambientColor);

    uniformString = tempString.arg(index).arg("diffuseColor");
    pShaderProgram->setUniformValue(uniformString.toStdString().c_str(), m_diffuesColor);

    uniformString = tempString.arg(index).arg("specularColor");
    pShaderProgram->setUniformValue(uniformString.toStdString().c_str(), m_specularColor);
}
