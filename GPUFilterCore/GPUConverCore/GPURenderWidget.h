#ifndef GPURENDERWIDGET_H
#define GPURENDERWIDGET_H

#include <QObject>
#include <QtOpenGL>
#include <QOpenGLWidget>
#include <QOpenGLFunctions_3_3_Core>
#include "GPUConverScene.h"
#include "OpenGLCore/GPUFilterScene.h"
#include "GPUFilterCore_global.h"

class GPUFilterMaterial;
class GPUFilterTexture;
class GPUFilterGeometryRect;
class GPUFILTERCORE_EXPORT GPURenderWidget : public QOpenGLWidget , public QOpenGLFunctions_3_3_Core
{
    Q_OBJECT

public:
    GPURenderWidget(QWidget* parent = nullptr);
    ~GPURenderWidget();

    void setImage(const QImage& image);
    void setYUVData(int type, const QVector<QByteArray>& yuvData, int width, int height);

protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

    void keyPressEvent(QKeyEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void wheelEvent(QWheelEvent *event) override;

private:
    GPUConverScene* m_pConverScene = nullptr;
    GPUFilterScene* m_pMainScene = nullptr;
    GPUFilterMaterial* m_pMaterial = nullptr;

    GPUFilterGeometryRect* m_pTVMesh = nullptr;
    GPUFilterGeometryRect* m_pTVMesh2 = nullptr;

    void createTestRectMesh(void);
    void createTestCubeMesh(void);
    void createTestLights(void);

    void createFloor(void);
    void createTVMesh(void);
    void createTVMesh2(void);

    QVector3D m_floorPostion;
};

#endif

