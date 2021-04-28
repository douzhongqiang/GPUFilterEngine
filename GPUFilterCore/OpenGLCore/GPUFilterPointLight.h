#ifndef GPUFILTERPOINTLIGHT_H
#define GPUFILTERPOINTLIGHT_H

#include <QObject>
#include <QVector3D>
#include "GPUFilterLightBase.h"

class GPUFilterPointLight : public GPUFilterLightBase
{
    Q_OBJECT

public:
    GPUFilterPointLight(QObject* parent = nullptr);
    ~GPUFilterPointLight();

    // get/set light postion
    void setLightPostion(const QVector3D& postionVec);
    QVector3D getLightPostion(void);

    // get/set attenuation
    void setAttenuationInfo(float constant, float linear, float quadratic);
    void getAttenuationInfo(float& constant, float& linear, float& quadratic);

    void processShader(QOpenGLShaderProgram* pShaderProgram, int index) override;

    int getType(void) override;

private:
    QVector3D m_LightPostion;

    float m_constant;
    float m_linear;
    float m_quadratic;
};

#endif
