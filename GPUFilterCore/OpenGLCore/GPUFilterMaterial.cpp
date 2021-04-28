#include "GPUFilterMaterial.h"

GPUFilterMaterial::GPUFilterMaterial(QObject* parent)
    :QObject(parent)
{

}

GPUFilterMaterial::~GPUFilterMaterial()
{

}

// set/get Color Enabled
void GPUFilterMaterial::setColorEnabled(bool isEnabled)
{
    m_isColorEnabled = isEnabled;
}

bool GPUFilterMaterial::getColorEnabled(void)
{
    return m_isColorEnabled;
}

// set/get Ambient
void GPUFilterMaterial::setAmbientColor(const QVector3D& color)
{
    m_ambientColor = color;
}

QVector3D GPUFilterMaterial::getAmbientColor(void)
{
    return m_diffuesColor;
}

// set/get Diffuse
void GPUFilterMaterial::setDiffuesColor(const QVector3D& color)
{
    m_specularColor = color;
}

QVector3D GPUFilterMaterial::getDiffuesColor(void)
{
    return m_specularColor;
}

// set/get Specular
void GPUFilterMaterial::setSpecularColor(const QVector3D& color)
{
    m_specularColor = color;
}

QVector3D GPUFilterMaterial::getSpecularColor(void)
{
    return m_specularColor;
}

// set/get Ambient texture
void GPUFilterMaterial::setAmbientTexture(QSharedPointer<GPUFilterTexture> texture)
{
    m_pAmbientTexture = texture;
    emit ambientTextureChanged();
}

QSharedPointer<GPUFilterTexture> GPUFilterMaterial::getAmbientTexture(void)
{
    return m_pAmbientTexture;
}

// set/get Diffuse
void GPUFilterMaterial::setDiffuesTexture(QSharedPointer<GPUFilterTexture> texture)
{
    m_pDiffuesTexture = texture;
    emit diffuesTextureChanged();
}

QSharedPointer<GPUFilterTexture> GPUFilterMaterial::getDiffuesTexture(void)
{
    return m_pDiffuesTexture;
}

// set/get Specular
void GPUFilterMaterial::setSpecularTexture(QSharedPointer<GPUFilterTexture> texture)
{
    m_pSpecularTexture = texture;
    emit specularTextureChanged();
}

QSharedPointer<GPUFilterTexture> GPUFilterMaterial::getSpecularTexture(void)
{
    return m_pSpecularTexture;
}

void GPUFilterMaterial::setShininess(float shininess)
{
    m_shininess = shininess;
}

float GPUFilterMaterial::getShininess(void)
{
    return m_shininess;
}

// Light Effect Enabled
void GPUFilterMaterial::setLightEffect(bool isEffect)
{
    m_isLightEffect = isEffect;
}

bool GPUFilterMaterial::isLightEffect(void)
{
    return m_isLightEffect;
}
