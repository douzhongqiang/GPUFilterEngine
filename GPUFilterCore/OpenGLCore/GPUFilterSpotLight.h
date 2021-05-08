#ifndef GPUFILTERSPOTLIGHT_H
#define GPUFILTERSPOTLIGHT_H

#include <QObject>
#include <QVector3D>
#include "GPUFilterLightBase.h"
#include "GPUFilterCore_global.h"

class GPUFILTERCORE_EXPORT GPUFilterSpotLight : public GPUFilterLightBase
{
    Q_OBJECT

public:
    GPUFilterSpotLight(QObject* parent = nullptr);
    ~GPUFilterSpotLight();

    // set/get light postion
    void setLightPostion(const QVector3D& postionVec);
    QVector3D getLightPostion(void);

    // set/get direction
    void setDirection(const QVector3D& direction);
    QVector3D getDirection(void);

    // set/get cutout infos
    void setCutoutInfo(float cutout, float cutoff);
    void getCutoutInfo(float& cutout, float& cutoff);

    virtual int getType(void);
    virtual void processShader(QOpenGLShaderProgram* pShaderProgram, int index);

private:
    QVector3D m_lightPostion;
    QVector3D m_direction;
    float  m_cutout;
    float  m_outerCutoff;
};

#endif
