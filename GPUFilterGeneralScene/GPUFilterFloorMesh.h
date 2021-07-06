#ifndef GPUFILTERFLOORMESH_H
#define GPUFILTERFLOORMESH_H

#include "3DExtras/GPUFilterGeometryRect.h"
#include "GPUFilterGeneralScene_global.h"

class GPUFILTERGENERALSCENE_EXPORT GPUFilterFloorMesh : public GPUFilterGeometryRect
{
    Q_OBJECT

public:
    GPUFilterFloorMesh(QObject* parent = nullptr);
    ~GPUFilterFloorMesh();

    void drawBefore(void) override;
    void drawAfter(void) override;
};

#endif
