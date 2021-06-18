#include "GPUFilterInvertedMesh.h"
#include "OpenGLCore/GPUFilterTool.h"

GPUFilterInvertedMesh::GPUFilterInvertedMesh(QObject* parent)
    :GPUFilterGeometryRect(parent)
{

}

GPUFilterInvertedMesh::~GPUFilterInvertedMesh()
{

}

void GPUFilterInvertedMesh::drawBefore(void)
{
    g_GPUFunc->glStencilFunc(GL_EQUAL, 1, 0xFF);
    g_GPUFunc->glStencilMask(0x00);

    this->getMaterial()->setFactor(QVector3D(0.4f, 0.4f, 0.4f));
}

void GPUFilterInvertedMesh::drawAfter(void)
{
    g_GPUFunc->glStencilFunc(GL_ALWAYS, 1, 0xFF);
    g_GPUFunc->glStencilMask(0xFF);
    this->getMaterial()->setFactor(QVector3D(1.0f, 1.0f, 1.0f));
}
