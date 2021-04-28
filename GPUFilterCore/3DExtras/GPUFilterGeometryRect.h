#ifndef GPUFILTERGEOMETRYRECT_H
#define GPUFILTERGEOMETRYRECT_H

#include "OpenGLCore/GPUFilterMesh.h"
class GPUFilterCamera;
class GPUFilterGeometryRect : public GPUFilterMesh
{
    Q_OBJECT

public:
    GPUFilterGeometryRect(QObject* parent = nullptr);
    ~GPUFilterGeometryRect();

    void setAlwayToBottom(bool isAlwayBottom);

    // Create
    bool create(void) override;
    // Draw
    void draw(void) override;

private:
    bool m_isAlawayBottom = false;
    void processAlwayBottomMatrix(void);
    void processModelMatrix(GPUFilterCamera* pCamera);
};

#endif
