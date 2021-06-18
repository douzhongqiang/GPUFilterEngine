#ifndef GPUFILTERVIDEOPLAYERSCENE_H
#define GPUFILTERVIDEOPLAYERSCENE_H

#include "OpenGLCore/GPUFilterScene.h"
#include <QObject>
#include <QtOpenGL>

class GPUFilterGeometryRect;
class GPUFilterMaterial;
class GPUFilterModel;
class GPUFilterVideoPlayerScene : public GPUFilterScene
{
    Q_OBJECT

public:
    GPUFilterVideoPlayerScene(QObject* parent = nullptr);
    ~GPUFilterVideoPlayerScene();

    void setYUVData(int type, const QVector<QByteArray>& yuvData, int width, int height);
    void loadAnimationModel(const QString& modelFilePath);
    void loadModel(const QString& modelFilePath);

    void render(void) override;
    void customMeshRenderBefore(void) override;

private:
    void initScene(void);

    void createTestRectMesh(void);
    void createTestCubeMesh(void);
    void createTestLights(void);

    void createFloor(void);
    void createTVMesh(void);
    void createTVMesh2(void);

    void createModelObject(void);
    void createAnimationModelObject(void);

    GPUFilterGeometryRect* m_pRectMesh = nullptr;
    GPUFilterGeometryRect* m_pFloorMesh = nullptr;
    GPUFilterGeometryRect* m_pTVMesh = nullptr;
    GPUFilterGeometryRect* m_pTVMesh2 = nullptr;
    GPUFilterMaterial* m_pMaterial = nullptr;

    // Model
    GPUFilterModel* m_pModel = nullptr;
    GPUFilterModel* m_pAnimationModel = nullptr;

    QVector3D m_floorPostion;
};

#endif
