#ifndef GPUFILTERINVERTEDMESH
#define GPUFILTERINVERTEDMESH

#include "3DExtras/GPUFilterGeometryRect.h"

class GPUFilterInvertedMesh : public GPUFilterGeometryRect
{
    Q_OBJECT

public:
    GPUFilterInvertedMesh(QObject* parent = nullptr);
    ~GPUFilterInvertedMesh();

    void drawBefore(void) override;
    void drawAfter(void) override;
};

#endif
