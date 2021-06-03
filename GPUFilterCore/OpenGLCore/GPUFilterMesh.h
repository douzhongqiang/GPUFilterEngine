#ifndef GPUFILTERMESH_H
#define GPUFILTERMESH_H

#include <QObject>
#include <QtOpenGL>
#include <QSharedPointer>
#include <QMatrix4x4>
#include "GPUFilterTexture.h"
#include "GPUFilterMaterial.h"
#include "GPUFilterCore_global.h"

class GPUFilterScene;
class GPUFILTERCORE_EXPORT GPUFilterMesh : public QObject
{
    Q_OBJECT
public:
    enum DataType
    {
        t_postion = 0,          // postion
        t_color,                // color
        t_normal,               // normal
        t_coord                 // texture coord
    };

public:
    GPUFilterMesh(QObject* parent = nullptr);
    ~GPUFilterMesh();

    // Set Data
    // Add Postion
    void addVertexPostion(const QVector3D& postion);
    // Add Color
    void addColor(const QVector3D& color);
    // Add Normal
    void addNormal(const QVector3D& normal);
    // Add Coord - For Texture2D Coord.z is 0
    void addCoord(const QVector3D& coord);
    // create Indices
    void createIndices(void);
    void setIndices(const QVector<GLuint>& indices);

    // set/get Model Matrix
    void setModelMartix(const QMatrix4x4& mat);
    QMatrix4x4 getModelMatrix(void);

    // set/get Material
    void setMaterial(QSharedPointer<GPUFilterMaterial> pMaterial);
    QSharedPointer<GPUFilterMaterial> getMaterial(void);

    // Create VBO, IBO, VAO
    virtual bool create(void);
    void release(void);
    bool hasCreated(void);

    void addTexture(QSharedPointer<GPUFilterTexture> pTexture);

    virtual void draw(void);

    // Get/Set Scene
    void setCurrentScene(GPUFilterScene* pScene);
    GPUFilterScene* getCurrentScene(void);

    // Process Material
    void processMaterial(void);
    void processMaterialTexture(void);

    // Visible
    void setVisible(bool isVisible);
    bool isVisible(void);

    int getVertexCount(void);

protected:
    QVector<QVector3D> m_posVec;
    QVector<QVector3D> m_colorVec;
    QVector<QVector3D> m_normalVec;
    QVector<QVector3D> m_coordVec;
    QVector<GLuint> m_indices;
    bool m_hasSetedIndices = false;

    GLuint m_nVBO = 0;
    GLuint m_nIBO = 0;      // Element Buffer Object
    GLuint m_nVAO = 0;
    bool m_hasCreated = false;

    bool m_isVisible = true;

    QList<QSharedPointer<GPUFilterTexture>> m_textures;

    QMatrix4x4 m_MMatrix;
    QSharedPointer<GPUFilterMaterial> m_pMaterial;

private:
    void createVBO(void);
    void createIBO(void);
    void createVAO(void);

    GPUFilterScene* m_pScene = nullptr;

private slots:
    void onAmbientTextureChanged(void);
    void onDiffuesTextureChanged(void);
    void onSpecularTextureChanged(void);

    void onExtraTexture1Changed(void);
    void onExtraTexture2Changed(void);
    void onExtraTexture3Changed(void);
};

#endif
