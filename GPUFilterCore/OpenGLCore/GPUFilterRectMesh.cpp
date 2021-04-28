#include "GPUFilterRectMesh.h"
#include "GPUFilterTool.h"

GPUFilterRectMesh::GPUFilterRectMesh(QObject* parent)
    :GPUFilterMesh(parent)
{

}

GPUFilterRectMesh::~GPUFilterRectMesh()
{

}

void GPUFilterRectMesh::init(void)
{
    // Add Vertex
    this->addVertexPostion(QVector3D(-1.0f, -1.0f,  0.0f));
    this->addVertexPostion(QVector3D(1.0f, -1.0f,  0.0f));
    this->addVertexPostion(QVector3D(1.0f,  1.0f,  0.0f));
    this->addVertexPostion(QVector3D(1.0f,  1.0f,  0.0f));
    this->addVertexPostion(QVector3D(-1.0f,  1.0f,  0.0f));
    this->addVertexPostion(QVector3D(-1.0f, -1.0f,  0.0f));

    // Add Color
    this->addColor(QVector3D(1.0f, 0.0f, 0.0f));
    this->addColor(QVector3D(1.0f, 0.0f, 0.0f));
    this->addColor(QVector3D(1.0f, 0.0f, 0.0f));
    this->addColor(QVector3D(1.0f, 0.0f, 0.0f));
    this->addColor(QVector3D(1.0f, 0.0f, 0.0f));
    this->addColor(QVector3D(1.0f, 0.0f, 0.0f));

    // Add Normal
    this->addNormal(QVector3D(0.0f,  0.0f,  1.0f));
    this->addNormal(QVector3D(0.0f,  0.0f,  1.0f));
    this->addNormal(QVector3D(0.0f,  0.0f,  1.0f));
    this->addNormal(QVector3D(0.0f,  0.0f,  1.0f));
    this->addNormal(QVector3D(0.0f,  0.0f,  1.0f));
    this->addNormal(QVector3D(0.0f,  0.0f,  1.0f));

    // Add Texture Coords
    this->addCoord(QVector3D(0.0f,  0.0f, 0.0f));
    this->addCoord(QVector3D(1.0f,  0.0f, 0.0f));
    this->addCoord(QVector3D(1.0f,  1.0f, 0.0f));
    this->addCoord(QVector3D(1.0f,  1.0f, 0.0f));
    this->addCoord(QVector3D(0.0f,  1.0f, 0.0f));
    this->addCoord(QVector3D(0.0f,  0.0f, 0.0f));

    this->create();
}

void GPUFilterRectMesh::draw(void)
{
    GPUFilterMesh::draw();
}
