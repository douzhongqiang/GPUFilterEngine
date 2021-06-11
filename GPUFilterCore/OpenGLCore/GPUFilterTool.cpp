#include "GPUFilterTool.h"
#include <QOpenGLContext>

GPUFilterTool::GPUFilterTool(QObject* parent)
    :QObject(parent)
{

}

GPUFilterTool::~GPUFilterTool()
{

}

GPUFilterTool* GPUFilterTool::getInstance(void)
{
    static GPUFilterTool instance;
    return &instance;
}

QOpenGLFunctions_3_3_Core* GPUFilterTool::getOpenGLFunc(void)
{
    return QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_3_3_Core>();
}

QMatrix4x4 GPUFilterTool::mix(const QMatrix4x4& x, const QMatrix4x4& y, float mixValue)
{
    return x * (1.0f - mixValue) + y * mixValue;
}

QVector3D GPUFilterTool::mix(const QVector3D& x, const QVector3D& y, float mixValue)
{
    return x * (1.0 - mixValue) + y * mixValue;
}
