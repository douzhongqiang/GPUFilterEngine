#ifndef GPUFILTERGEOMETRYCUBEBOX_H
#define GPUFILTERGEOMETRYCUBEBOX_H

#include <QObject>
#include "OpenGLCore/GPUFilterMesh.h"
#include "GPUFilterCore_global.h"

class GPUFILTERCORE_EXPORT GPUFilterGeometry : public GPUFilterMesh
{
    Q_OBJECT

public:
    GPUFilterGeometry(QObject* parent = nullptr);
    ~GPUFilterGeometry();

    bool create(void) override;

private:
    void initPos(void);
    void initNormal(void);
    void initCoord(void);
};

#endif
