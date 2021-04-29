#include "GPUFilterFlashLight.h"
#include "GPUFilterCamera.h"
#include "GPUFilterScene.h"

GPUFilterFlashLight::GPUFilterFlashLight(QObject* parent)
    :GPUFilterSpotLight(parent)
{

}

GPUFilterFlashLight::~GPUFilterFlashLight()
{

}

void GPUFilterFlashLight::processShader(QOpenGLShaderProgram* pShaderProgram, int index)
{
    if (m_pScene && m_pScene->getCamera())
    {
        QVector3D cameraPos = m_pScene->getCamera()->getCameraPostion();
        QVector3D cameraFront = m_pScene->getCamera()->getCameraFront();

        this->setLightPostion(cameraPos);
        this->setDirection(cameraFront);
    }

    GPUFilterSpotLight::processShader(pShaderProgram, index);
}
