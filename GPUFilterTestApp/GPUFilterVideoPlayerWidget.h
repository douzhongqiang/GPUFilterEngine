#ifndef GPUFILTERVIDEOPLAYERWIDGET_H
#define GPUFILTERVIDEOPLAYERWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions_3_3_Core>
class GPUFilterVideoPlayerScene;
class QTimer;
class GPUFilterFBO;
class GPUFilterPostProcessScene;
class GPUFilterPBO2;

class GPUFilterVideoPlayerWidget : public QOpenGLWidget , public QOpenGLFunctions_3_3_Core
{
    Q_OBJECT

public:
    GPUFilterVideoPlayerWidget(QWidget* parent = nullptr);
    ~GPUFilterVideoPlayerWidget();

    void setYUVData(int type, const QVector<QByteArray>& yuvData, int width, int height);
    void loadAnimationModel(const QString& modelFilePath);
    void loadModel(const QString& modelFilePath);

    QImage grapImage(int width, int height);

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
    GPUFilterVideoPlayerScene* m_pScene = nullptr;
    GPUFilterPostProcessScene* m_pPostProcessScene = nullptr;
    GPUFilterPostProcessScene* m_pYUVConvertScene = nullptr;
    GPUFilterPBO2* m_pPackPBO = nullptr;

    // Update About
    QTimer* m_pTimer = nullptr;
    void initTimer(void);

private slots:
    void onTimeout(void);
};

#endif
