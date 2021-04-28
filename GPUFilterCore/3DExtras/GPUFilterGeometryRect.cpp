#include "GPUFilterGeometryRect.h"
#include "OpenGLCore/GPUFilterScene.h"
#include "OpenGLCore/GPUFilterCamera.h"
#include <QOpenGLShaderProgram>

GPUFilterGeometryRect::GPUFilterGeometryRect(QObject* parent)
    :GPUFilterMesh(parent)
{

}

GPUFilterGeometryRect::~GPUFilterGeometryRect()
{

}

void GPUFilterGeometryRect::setAlwayToBottom(bool isAlwayBottom)
{
    m_isAlawayBottom = isAlwayBottom;
}

bool GPUFilterGeometryRect::create(void)
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

    return GPUFilterMesh::create();
}

void GPUFilterGeometryRect::processAlwayBottomMatrix(void)
{
    if (!m_isAlawayBottom)
        return;

    GPUFilterScene* pScene = getCurrentScene();
    if (pScene == nullptr)
        return;

    QOpenGLShaderProgram* pShaderProgram = pScene->getShaderProgram();
    GPUFilterCamera* pCamera = pScene->getCamera();

    if (pShaderProgram == nullptr || pCamera == nullptr)
        return;

    //QMatrix3x3 tempMatrix = pCamera->getVMatrix().toGenericMatrix<3, 3>();
    pShaderProgram->setUniformValue("V", QMatrix4x4(/*tempMatrix*/));

    processModelMatrix(pCamera);
}

void GPUFilterGeometryRect::processModelMatrix(GPUFilterCamera* pCamera)
{
    m_MMatrix.setToIdentity();
    m_MMatrix.translate(0, 0, -80.0f);
    int width, height;
    pCamera->getViewport(width, height);

    float viewHeight = qTan(qDegreesToRadians(pCamera->getPerAngle() / 2.0f)) * (80.0) * 2;
    float viewWidth = viewHeight * (width * 1.0 / height);

    float scaleX = viewWidth / 2.0f;
    float scaleY = viewHeight / 2.0f;

    if (this->getMaterial() && this->getMaterial()->getDiffuesTexture())
    {
        QSharedPointer<GPUFilterTexture> pTexture = this->getMaterial()->getDiffuesTexture();
        int srcImageWidth = pTexture->getWidth();
        int srcImageHeight = pTexture->getHeight();

        if (srcImageWidth != 0 && srcImageHeight != 0)
        {
            float tempScaleX = viewWidth * 1.0 / srcImageWidth;
            float tempScaleY = viewHeight * 1.0 / srcImageHeight;

            if (tempScaleX < tempScaleY)
                scaleY = viewWidth / 2.0 * (srcImageHeight * 1.0 / srcImageWidth);
            else
                scaleX = viewHeight / 2.0 * (srcImageWidth * 1.0 / srcImageHeight);
        }
    }

    m_MMatrix.scale(scaleX, -scaleY, 1.0f);
}

void GPUFilterGeometryRect::draw(void)
{
    processAlwayBottomMatrix();

    GPUFilterMesh::draw();
}
