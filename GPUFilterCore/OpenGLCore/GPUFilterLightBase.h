#ifndef GPUFILTERLIGHTBASE_H
#define GPUFILTERLIGHTBASE_H

#include <QObject>
#include <QVector3D>
#include <QOpenGLShaderProgram>

class GPUFilterLightBase : public QObject
{
    Q_OBJECT

public:
    enum LightType
    {
        t_DirectionLight = 0,
        t_PointLight
    };

public:
    GPUFilterLightBase(QObject* parent = nullptr);
    ~GPUFilterLightBase();

    // set/get Ambient
    void setAmbientColor(const QVector3D& color);
    QVector3D getAmbientColor(void);

    // set/get Diffuse
    void setDiffuesColor(const QVector3D& color);
    QVector3D getDiffuesColor(void);

    // set/get Specular
    void setSpecularColor(const QVector3D& color);
    QVector3D getSpecularColor(void);

    virtual int getType(void);
    virtual void processShader(QOpenGLShaderProgram* pShaderProgram, int index);

protected:
    QVector3D m_ambientColor;
    QVector3D m_diffuesColor;
    QVector3D m_specularColor;

    LightType m_type = t_DirectionLight;
    bool m_isEnabled = true;
};

#endif
