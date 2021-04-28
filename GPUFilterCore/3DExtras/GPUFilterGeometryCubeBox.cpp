#include "GPUFilterGeometryCubeBox.h"

GPUFilterGeometry::GPUFilterGeometry(QObject* parent)
    :GPUFilterMesh(parent)
{

}

GPUFilterGeometry::~GPUFilterGeometry()
{

}

bool GPUFilterGeometry::create(void)
{
    initPos();
    initNormal();
    initCoord();

    return GPUFilterMesh::create();
}

void GPUFilterGeometry::initPos(void)
{
    this->addVertexPostion(QVector3D(-1.0f, -1.0f, -1.0f));
    this->addVertexPostion(QVector3D(1.0f, -1.0f, -1.0f));
    this->addVertexPostion(QVector3D(1.0f,  1.0f, -1.0f));
    this->addVertexPostion(QVector3D(1.0f,  1.0f, -1.0f));
    this->addVertexPostion(QVector3D(-1.0f,  1.0f, -1.0f));
    this->addVertexPostion(QVector3D(-1.0f, -1.0f, -1.0f));

    this->addVertexPostion(QVector3D(-1.0f, -1.0f,  1.0f));
    this->addVertexPostion(QVector3D(1.0f, -1.0f,  1.0f));
    this->addVertexPostion(QVector3D(1.0f,  1.0f,  1.0f));
    this->addVertexPostion(QVector3D(1.0f,  1.0f,  1.0f));
    this->addVertexPostion(QVector3D(-1.0f,  1.0f,  1.0f));
    this->addVertexPostion(QVector3D(-1.0f, -1.0f,  1.0f));

    this->addVertexPostion(QVector3D(-1.0f,  1.0f,  1.0f));
    this->addVertexPostion(QVector3D(-1.0f,  1.0f, -1.0f));
    this->addVertexPostion(QVector3D(-1.0f, -1.0f, -1.0f));
    this->addVertexPostion(QVector3D(-1.0f, -1.0f, -1.0f));
    this->addVertexPostion(QVector3D(-1.0f, -1.0f,  1.0f));
    this->addVertexPostion(QVector3D(-1.0f,  1.0f,  1.0f));

    this->addVertexPostion(QVector3D(1.0f,  1.0f,  1.0f));
    this->addVertexPostion(QVector3D(1.0f,  1.0f, -1.0f));
    this->addVertexPostion(QVector3D(1.0f, -1.0f, -1.0f));
    this->addVertexPostion(QVector3D(1.0f, -1.0f, -1.0f));
    this->addVertexPostion(QVector3D(1.0f, -1.0f,  1.0f));
    this->addVertexPostion(QVector3D(1.0f,  1.0f,  1.0f));

    this->addVertexPostion(QVector3D(-1.0f, -1.0f, -1.0f));
    this->addVertexPostion(QVector3D(1.0f, -1.0f, -1.0f));
    this->addVertexPostion(QVector3D(1.0f, -1.0f,  1.0f));
    this->addVertexPostion(QVector3D(1.0f, -1.0f,  1.0f));
    this->addVertexPostion(QVector3D(-1.0f, -1.0f,  1.0f));
    this->addVertexPostion(QVector3D(-1.0f, -1.0f, -1.0f));

    this->addVertexPostion(QVector3D(-1.0f,  1.0f, -1.0f));
    this->addVertexPostion(QVector3D(1.0f,  1.0f, -1.0f));
    this->addVertexPostion(QVector3D(1.0f,  1.0f,  1.0f));
    this->addVertexPostion(QVector3D(1.0f,  1.0f,  1.0f));
    this->addVertexPostion(QVector3D(-1.0f,  1.0f,  1.0f));
    this->addVertexPostion(QVector3D(-1.0f,  1.0f, -1.0f));
}

void GPUFilterGeometry::initNormal(void)
{
    this->addNormal(QVector3D(0.0f,  0.0f, -1.0f));
    this->addNormal(QVector3D(0.0f,  0.0f, -1.0f));
    this->addNormal(QVector3D(0.0f,  0.0f, -1.0f));
    this->addNormal(QVector3D(0.0f,  0.0f, -1.0f));
    this->addNormal(QVector3D(0.0f,  0.0f, -1.0f));
    this->addNormal(QVector3D(0.0f,  0.0f, -1.0f));

    this->addNormal(QVector3D(0.0f,  0.0f,  1.0f));
    this->addNormal(QVector3D(0.0f,  0.0f,  1.0f));
    this->addNormal(QVector3D(0.0f,  0.0f,  1.0f));
    this->addNormal(QVector3D(0.0f,  0.0f,  1.0f));
    this->addNormal(QVector3D(0.0f,  0.0f,  1.0f));
    this->addNormal(QVector3D(0.0f,  0.0f,  1.0f));

    this->addNormal(QVector3D(-1.0f,  0.0f,  0.0f));
    this->addNormal(QVector3D(-1.0f,  0.0f,  0.0f));
    this->addNormal(QVector3D(-1.0f,  0.0f,  0.0f));
    this->addNormal(QVector3D(-1.0f,  0.0f,  0.0f));
    this->addNormal(QVector3D(-1.0f,  0.0f,  0.0f));
    this->addNormal(QVector3D(-1.0f,  0.0f,  0.0f));

    this->addNormal(QVector3D(1.0f,  0.0f,  0.0f));
    this->addNormal(QVector3D(1.0f,  0.0f,  0.0f));
    this->addNormal(QVector3D(1.0f,  0.0f,  0.0f));
    this->addNormal(QVector3D(1.0f,  0.0f,  0.0f));
    this->addNormal(QVector3D(1.0f,  0.0f,  0.0f));
    this->addNormal(QVector3D(1.0f,  0.0f,  0.0f));

    this->addNormal(QVector3D(0.0f, -1.0f,  0.0f));
    this->addNormal(QVector3D(0.0f, -1.0f,  0.0f));
    this->addNormal(QVector3D(0.0f, -1.0f,  0.0f));
    this->addNormal(QVector3D(0.0f, -1.0f,  0.0f));
    this->addNormal(QVector3D(0.0f, -1.0f,  0.0f));
    this->addNormal(QVector3D(0.0f, -1.0f,  0.0f));

    this->addNormal(QVector3D(0.0f,  1.0f,  0.0f));
    this->addNormal(QVector3D(0.0f,  1.0f,  0.0f));
    this->addNormal(QVector3D(0.0f,  1.0f,  0.0f));
    this->addNormal(QVector3D(0.0f,  1.0f,  0.0f));
    this->addNormal(QVector3D(0.0f,  1.0f,  0.0f));
    this->addNormal(QVector3D(0.0f,  1.0f,  0.0f));
}

void GPUFilterGeometry::initCoord(void)
{
    this->addCoord(QVector3D(0.0f,  0.0f, 0.0f));
    this->addCoord(QVector3D(1.0f,  0.0f, 0.0f));
    this->addCoord(QVector3D(1.0f,  1.0f, 0.0f));
    this->addCoord(QVector3D(1.0f,  1.0f, 0.0f));
    this->addCoord(QVector3D(0.0f,  1.0f, 0.0f));
    this->addCoord(QVector3D(0.0f,  0.0f, 0.0f));

    this->addCoord(QVector3D(0.0f,  0.0f, 0.0f));
    this->addCoord(QVector3D(1.0f,  0.0f, 0.0f));
    this->addCoord(QVector3D(1.0f,  1.0f, 0.0f));
    this->addCoord(QVector3D(1.0f,  1.0f, 0.0f));
    this->addCoord(QVector3D(0.0f,  1.0f, 0.0f));
    this->addCoord(QVector3D(0.0f,  0.0f, 0.0f));

    this->addCoord(QVector3D(1.0f,  0.0f, 0.0f));
    this->addCoord(QVector3D(1.0f,  1.0f, 0.0f));
    this->addCoord(QVector3D(0.0f,  1.0f, 0.0f));
    this->addCoord(QVector3D(0.0f,  1.0f, 0.0f));
    this->addCoord(QVector3D(0.0f,  0.0f, 0.0f));
    this->addCoord(QVector3D(1.0f,  0.0f, 0.0f));

    this->addCoord(QVector3D(1.0f,  0.0f, 0.0f));
    this->addCoord(QVector3D(1.0f,  1.0f, 0.0f));
    this->addCoord(QVector3D(0.0f,  1.0f, 0.0f));
    this->addCoord(QVector3D(0.0f,  1.0f, 0.0f));
    this->addCoord(QVector3D(0.0f,  0.0f, 0.0f));
    this->addCoord(QVector3D(1.0f,  0.0f, 0.0f));

    this->addCoord(QVector3D(0.0f,  1.0f, 0.0f));
    this->addCoord(QVector3D(1.0f,  1.0f, 0.0f));
    this->addCoord(QVector3D(1.0f,  0.0f, 0.0f));
    this->addCoord(QVector3D(1.0f,  0.0f, 0.0f));
    this->addCoord(QVector3D(0.0f,  0.0f, 0.0f));
    this->addCoord(QVector3D(0.0f,  1.0f, 0.0f));

    this->addCoord(QVector3D(0.0f,  1.0f, 0.0f));
    this->addCoord(QVector3D(1.0f,  1.0f, 0.0f));
    this->addCoord(QVector3D(1.0f,  0.0f, 0.0f));
    this->addCoord(QVector3D(1.0f,  0.0f, 0.0f));
    this->addCoord(QVector3D(0.0f,  0.0f, 0.0f));
    this->addCoord(QVector3D(0.0f,  1.0f, 0.0f));
}
