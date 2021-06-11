#ifndef GPUFILTERTOOL_H
#define GPUFILTERTOOL_H

#include <QObject>
#include <QMatrix4x4>
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

    // Mix x And y, mixValue (0 ~ 1)
    QMatrix4x4 mix(const QMatrix4x4& x, const QMatrix4x4& y, float mixValue);
    QVector3D mix(const QVector3D& x, const QVector3D& y, float mixValue);

private:
    GPUFilterTool(QObject* parent = nullptr);
    ~GPUFilterTool();
};

#endif
