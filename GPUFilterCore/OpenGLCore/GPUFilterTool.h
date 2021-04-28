#ifndef GPUFILTERTOOL_H
#define GPUFILTERTOOL_H

#include <QObject>
#include <QOpenGLFunctions_3_3_Core>

#define g_GPUFilterTool GPUFilterTool::getInstance()
#define g_GPUFunc g_GPUFilterTool->getOpenGLFunc()

class GPUFilterTool : public QObject
{
public:
    static GPUFilterTool* getInstance(void);

    // Get OpenGL Function, Used OpenGL 3.3
    QOpenGLFunctions_3_3_Core* getOpenGLFunc(void);

private:
    GPUFilterTool(QObject* parent = nullptr);
    ~GPUFilterTool();
};

#endif
