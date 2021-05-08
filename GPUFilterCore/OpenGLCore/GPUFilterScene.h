#ifndef GPUFILTERSCENE_H
#define GPUFILTERSCENE_H

#include <QObject>
#include <QtOpenGL>
#include "GPUFilterCamera.h"
#include "GPUFilterRectMesh.h"
#include "GPUFilterShaderProgram.h"
#include "GPUFilterMesh.h"
#include "GPUFilterLightBase.h"
#include "GPUFilterCore_global.h"

class GPUFILTERCORE_EXPORT GPUFilterScene : public QObject
{
    Q_OBJECT

public:
    GPUFilterScene(QObject* parent = nullptr);
    ~GPUFilterScene();

    virtual void init(void);
    virtual void render(void);
    virtual void resize(int w, int h);

    // Add And Remove Mesh
    void addMesh(GPUFilterMesh* pMesh);
    void removeMesh(GPUFilterMesh* pMesh);

    // Add Lights
    void addLight(GPUFilterLightBase* pLight);

    // Get Shader Program
    QOpenGLShaderProgram* getShaderProgram(void);
    // Get Camera
    GPUFilterCamera* getCamera(void);

    // Set/Get Background Color
    void setBgColor(const QColor& color);
    QColor getBgColor(void);

protected:
    GPUFilterShaderProgram* m_pShaderProgram = nullptr;
    GPUFilterCamera* m_pCamera = nullptr;

    QVector<GPUFilterMesh*> m_meshVec;
    QVector<GPUFilterLightBase*> m_lightVec;

    bool m_isInited = false;

    void effectLights(void);
    void effectLights(GPUFilterLightBase* pLight, int index);

    QColor m_bgColor;
};

#endif
