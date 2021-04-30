#ifndef GPUFILTERMATERIAL_H
#define GPUFILTERMATERIAL_H

#include <QObject>
#include <QtOpenGL>
#include <QVector3D>
#include <QSharedPointer>
#include "GPUFilterTexture.h"

class GPUFilterMaterial : public QObject
{
    Q_OBJECT

public:
    GPUFilterMaterial(QObject* parent = nullptr);
    ~GPUFilterMaterial();

    // set/get Color Enabled
    void setColorEnabled(bool isEnabled);
    bool getColorEnabled(void);

    // set/get Ambient
    void setAmbientColor(const QVector3D& color);
    QVector3D getAmbientColor(void);
    // set/get Diffuse
    void setDiffuesColor(const QVector3D& color);
    QVector3D getDiffuesColor(void);
    // set/get Specular
    void setSpecularColor(const QVector3D& color);
    QVector3D getSpecularColor(void);

    // set/get Ambient texture
    void setAmbientTexture(QSharedPointer<GPUFilterTexture> texture);
    QSharedPointer<GPUFilterTexture> getAmbientTexture(void);
    // set/get Diffuse
    void setDiffuesTexture(QSharedPointer<GPUFilterTexture> texture);
    QSharedPointer<GPUFilterTexture> getDiffuesTexture(void);
    // set/get Specular
    void setSpecularTexture(QSharedPointer<GPUFilterTexture> texture);
    QSharedPointer<GPUFilterTexture> getSpecularTexture(void);

    // set/get shininess
    void setShininess(float shininess);
    float getShininess(void);

    // Light Effect Enabled
    void setLightEffect(bool isEffect);
    bool isLightEffect(void);

    // get/set Used Extra Texture
    void setExtraTextureEnabled(bool isEnabled);
    bool isExtraTextureEnabled(void);

    // set/get Extra Texture
    void setExtraTexture1(QSharedPointer<GPUFilterTexture> texture);
    void setExtraTexture2(QSharedPointer<GPUFilterTexture> texture);
    void setExtraTexture3(QSharedPointer<GPUFilterTexture> texture);
    QSharedPointer<GPUFilterTexture> getExtraTexture1(void);
    QSharedPointer<GPUFilterTexture> getExtraTexture2(void);
    QSharedPointer<GPUFilterTexture> getExtraTexture3(void);

private:
    bool m_isColorEnabled = true;

    // Color
    QVector3D m_ambientColor;
    QVector3D m_diffuesColor;
    QVector3D m_specularColor;

    // Texture
    QSharedPointer<GPUFilterTexture> m_pAmbientTexture;
    QSharedPointer<GPUFilterTexture> m_pDiffuesTexture;
    QSharedPointer<GPUFilterTexture> m_pSpecularTexture;

    float m_shininess = 32.0f;
    bool m_isLightEffect = true;

    // Extra Texture
    bool m_isUsedExtraTexture = false;
    QSharedPointer<GPUFilterTexture> m_pExtraTexture1;
    QSharedPointer<GPUFilterTexture> m_pExtraTexture2;
    QSharedPointer<GPUFilterTexture> m_pExtraTexture3;

signals:
    void ambientTextureChanged(void);
    void diffuesTextureChanged(void);
    void specularTextureChanged(void);

    void extraTexture1Changed(void);
    void extraTexture2Changed(void);
    void extraTexture3Changed(void);
};

#endif
