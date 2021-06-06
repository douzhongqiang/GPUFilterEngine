#include "GPUFilterBoneMesh.h"
#include "OpenGLCore/GPUFilterTool.h"

GPUFilterIVector4D::GPUFilterIVector4D()
{

}

GPUFilterIVector4D::GPUFilterIVector4D(int x, int y, int z, int w)
{
    v[0] = x;
    v[1] = y;
    v[2] = z;
    v[3] = w;
}

GPUFilterIVector4D::~GPUFilterIVector4D()
{

}

int GPUFilterIVector4D::x() const
{
    return v[0];
}

int GPUFilterIVector4D::y() const
{
    return v[1];
}

int GPUFilterIVector4D::z() const
{
    return v[2];
}

int GPUFilterIVector4D::w() const
{
    return v[3];
}

void GPUFilterIVector4D::setX(int x)
{
    v[0] = x;
}

void GPUFilterIVector4D::setY(int y)
{
    v[1] = y;
}

void GPUFilterIVector4D::setZ(int z)
{
    v[2] = z;
}

void GPUFilterIVector4D::setW(int w)
{
    v[3] = w;
}

// ==============================================================================
GPUFilterBoneMesh::GPUFilterBoneMesh(QObject* parent)
    :GPUFilterMesh(parent)
    , m_boneLocalBoneId(t_customUser)
    , m_weightLocaWeightId(t_customUser + 1)
    , m_nVectorSize(4)
{

}

GPUFilterBoneMesh::~GPUFilterBoneMesh()
{

}

int GPUFilterBoneMesh::customVBOSize(void)
{
    int size = m_boneIds.size() * sizeof(GPUFilterIVector4D) + m_weights.size() * sizeof(QVector4D);
    return size;
}

void GPUFilterBoneMesh::customMapVBOData(char* ptr)
{
    int index = 0;
    memcpy((char*)ptr + index, m_boneIds.data(), sizeof (GPUFilterIVector4D) * m_boneIds.size());

    index += sizeof (GPUFilterIVector4D) * m_boneIds.size();
    memcpy((char*)ptr + index, m_weights.data(), sizeof (QVector4D) * m_weights.size());
}

void GPUFilterBoneMesh::customVBODataCreate(int index)
{
    // Set Bone Atttibute
    g_GPUFunc->glEnableVertexAttribArray(m_boneLocalBoneId);
    g_GPUFunc->glVertexAttribIPointer(m_boneLocalBoneId, 4, GL_INT, sizeof(GPUFilterIVector4D), (void*)index);

    // Set Weight Attribute
    index += sizeof(GPUFilterIVector4D);
    g_GPUFunc->glEnableVertexAttribArray(m_weightLocaWeightId);
    g_GPUFunc->glVertexAttribPointer(m_weightLocaWeightId, 4, GL_FLOAT, GL_FALSE, sizeof(QVector4D), (void*)index);
}

void GPUFilterBoneMesh::addOneDefBoneInfo(void)
{
    GPUFilterIVector4D iBoneInfo(-1, -1, -1, -1);
    m_boneIds.push_back(iBoneInfo);

    QVector4D weightInfo(0.0f, 0.0f, 0.0f, 0.0f);
    m_weights.push_back(weightInfo);
}
