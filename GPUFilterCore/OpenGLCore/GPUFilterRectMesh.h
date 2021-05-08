#ifndef GPUFILTERRECTMESH_H
#define GPUFILTERRECTMESH_H

#include <QObject>
#include "GPUFilterMesh.h"
#include "GPUFilterCore_global.h"

class GPUFILTERCORE_EXPORT GPUFilterRectMesh : public GPUFilterMesh
{
    Q_OBJECT

public:
    GPUFilterRectMesh(QObject* parent = nullptr);
    ~GPUFilterRectMesh();

    // init
    void init(void);

    void draw(void) override;
};

#endif
