#ifndef GPUFILTERFLASHLIGHT_H
#define GPUFILTERFLASHLIGHT_H

#include <QObject>
#include <QVector3D>
#include "GPUFilterSpotLight.h"
#include "GPUFilterCore_global.h"

class GPUFILTERCORE_EXPORT GPUFilterFlashLight : public GPUFilterSpotLight
{
    Q_OBJECT

public:
    GPUFilterFlashLight(QObject* parent = nullptr);
    ~GPUFilterFlashLight();

    void processShader(QOpenGLShaderProgram* pShaderProgram, int index) override;
};

#endif
