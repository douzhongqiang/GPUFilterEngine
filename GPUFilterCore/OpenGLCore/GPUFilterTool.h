#ifndef GPUFILTERTOOL_H
#define GPUFILTERTOOL_H

#include <QObject>
#include <QOpenGLFunctions_3_3_Core>
#include "GPUFilterCore_global.h"

#define g_GPUFilterTool GPUFilterTool::getInstance()
#define g_GPUFunc g_GPUFilterTool->getOpenGLFunc()

class GPUFILTERCORE_EXPORT GPUFilterTool : public QObject
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
