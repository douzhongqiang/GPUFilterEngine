#ifndef GPUFILTERVIDEOPLAYERWIDGET_H
#define GPUFILTERVIDEOPLAYERWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions_3_3_Core>
class GPUFilterVideoPlayerScene;

class GPUFilterVideoPlayerWidget : public QOpenGLWidget , public QOpenGLFunctions_3_3_Core
{
    Q_OBJECT

public:
    GPUFilterVideoPlayerWidget(QWidget* parent = nullptr);
    ~GPUFilterVideoPlayerWidget();

    void setYUVData(int type, const QVector<QByteArray>& yuvData, int width, int height);
    void loadModel(const QString& modelFilePath);

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
};

#endif
