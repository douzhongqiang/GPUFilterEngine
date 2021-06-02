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
    void loadModel(const QString& modelFilePath);

    void render(void) override;

private:
    void initScene(void);

    void createTestRectMesh(void);
    void createTestCubeMesh(void);
    void createTestLights(void);

    void createFloor(void);
    void createTVMesh(void);
    void createTVMesh2(void);

    void createModelObject(void);

    GPUFilterGeometryRect* m_pRectMesh = nullptr;
    GPUFilterGeometryRect* m_pFloorMesh = nullptr;
    GPUFilterGeometryRect* m_pTVMesh = nullptr;
    GPUFilterGeometryRect* m_pTVMesh2 = nullptr;
    GPUFilterMaterial* m_pMaterial = nullptr;

    GPUFilterModel* m_pModel = nullptr;

    QVector3D m_floorPostion;
};

#endif
