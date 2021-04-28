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
