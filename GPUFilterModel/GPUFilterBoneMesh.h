#ifndef GPUFILTERBONEMESH_H
#define GPUFILTERBONEMESH_H

#include <QObject>
#include <QVector4D>
#include "OpenGLCore/GPUFilterMesh.h"

class GPUFilterIVector4D
{
public:
    GPUFilterIVector4D();
    GPUFilterIVector4D(int x, int y, int z, int w);
    ~GPUFilterIVector4D();

    int x() const;
    int y() const;
    int z() const;
    int w() const;

    void setX(int x);
    void setY(int y);
    void setZ(int z);
    void setW(int w);

private:
    int v[4];
};

// ==============================================================================
class GPUFilterBoneMesh : public GPUFilterMesh
{
    Q_OBJECT

public:
    GPUFilterBoneMesh(QObject* parent = nullptr);
    ~GPUFilterBoneMesh();

    void addOneDefBoneInfo(void);

protected:
    QVector<GPUFilterIVector4D> m_boneIds;
    QVector<QVector4D> m_weights;

    const int m_boneLocalBoneId;
    const int m_weightLocaWeightId;
    const int m_nVectorSize;

    int customVBOSize(void) override;
    void customMapVBOData(char* ptr) override;
    void customVBODataCreate(int index) override;
};

#endif
