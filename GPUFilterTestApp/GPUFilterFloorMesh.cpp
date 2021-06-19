#include "GPUFilterFloorMesh.h"
#include "OpenGLCore/GPUFilterTool.h"

GPUFilterFloorMesh::GPUFilterFloorMesh(QObject* parent)
    :GPUFilterGeometryRect(parent)
{

}

GPUFilterFloorMesh::~GPUFilterFloorMesh()
{

}

void GPUFilterFloorMesh::drawBefore(void)
{
    g_GPUFunc->glStencilFunc(GL_ALWAYS, 1, 0xFF);
    g_GPUFunc->glStencilMask(0xFF);
    g_GPUFunc->glDepthMask(GL_FALSE);
}

void GPUFilterFloorMesh::drawAfter(void)
{
    g_GPUFunc->glDepthFunc(GL_LESS);
    g_GPUFunc->glDepthMask(GL_TRUE);
}
