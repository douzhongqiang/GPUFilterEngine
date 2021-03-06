#include "GPUFilterVideoPlayerScene.h"
#include "3DExtras/GPUFilterGeometryRect.h"
#include "3DExtras/GPUFilterGeometryCubeBox.h"
#include "OpenGLCore/GPUFilterMaterial.h"
#include "OpenGLCore/GPUFilterTexture.h"
#include "OpenGLCore/GPUFilterDirectionLight.h"
#include "OpenGLCore/GPUFilterPointLight.h"
#include "OpenGLCore/GPUFilterSpotLight.h"
#include "OpenGLCore/GPUFilterFlashLight.h"
#include "OpenGLCore/GPUFilterTool.h"
#include "GPUFilterFloorMesh.h"
#include "GPUFilterInvertedMesh.h"
#include "GPUFilterModel.h"
#include <QApplication>

GPUFilterVideoPlayerScene::GPUFilterVideoPlayerScene(QObject* parent)
    :GPUFilterScene(parent)
    , m_floorPostion(0.0f, -5.0f, -20.0f)
{
    initScene();
}

GPUFilterVideoPlayerScene::~GPUFilterVideoPlayerScene()
{

}

void GPUFilterVideoPlayerScene::render(void)
{
    return GPUFilterScene::render();
}

void GPUFilterVideoPlayerScene::customMeshRenderBefore(void)
{
    if (m_pAnimationModel)
        m_pAnimationModel->draw();
}

void GPUFilterVideoPlayerScene::initScene(void)
{
    createTestRectMesh();
    createTestCubeMesh();
    createTestLights();

    // Create TV Mesh
    createTVMesh();
    createFloor();
    createTVMesh2();

    // Create Object
    createModelObject();
    createAnimationModelObject();
}

void GPUFilterVideoPlayerScene::createTestRectMesh(void)
{
    m_pRectMesh = new GPUFilterGeometryRect;
    this->addMesh(m_pRectMesh);

    m_pMaterial = new GPUFilterMaterial;
    m_pRectMesh->setMaterial(QSharedPointer<GPUFilterMaterial>(m_pMaterial));
    m_pRectMesh->setAlwayToBottom(true);
    m_pMaterial->setLightEffect(false);

    // Set Image
    m_pMaterial->setColorEnabled(false);

    QString appDir = qApp->applicationDirPath() + "/";

    QImage image(appDir + "./green-5919790_1280.jpg");
    QSharedPointer<GPUFilterTexture> pTexture(new GPUFilterTexture(this));
    pTexture->setImage(image);
    pTexture->setObjectName("Texture1");

    QImage image2(appDir + "./city-4478471_1280.jpg");
    QSharedPointer<GPUFilterTexture> pTexture2(new GPUFilterTexture(this));
    pTexture2->setImage(image2);
    pTexture2->setObjectName("Texture2");

    QImage image3(appDir + "./digital-paper-4906487_1280.jpg");
    QSharedPointer<GPUFilterTexture> pTexture3(new GPUFilterTexture(this));
    pTexture3->setImage(image3);
    pTexture3->setObjectName("Texture3");

    m_pMaterial->setAmbientTexture(pTexture);
    m_pMaterial->setDiffuesTexture(pTexture2);
    m_pMaterial->setSpecularTexture(pTexture3);
}

void GPUFilterVideoPlayerScene::createTestCubeMesh(void)
{
    GPUFilterGeometry* pMesh = new GPUFilterGeometry;
    this->addMesh(pMesh);
    GPUFilterMaterial* pMaterial = new GPUFilterMaterial;
    pMaterial->setColorEnabled(false);

    QString appDir = qApp->applicationDirPath() + "/";

    // Set Texture
    QSharedPointer<GPUFilterTexture> pTexture1(new GPUFilterTexture(this));
    pTexture1->setImage(QImage(appDir + "./container2.png"));
    pMaterial->setAmbientTexture(pTexture1);
    pMaterial->setDiffuesTexture(pTexture1);
    QSharedPointer<GPUFilterTexture> pTexture2(new GPUFilterTexture(this));
    pTexture2->setImage(QImage(appDir + "./container2_specular.png"));
    pMaterial->setSpecularTexture(pTexture2);

    pMesh->setMaterial(QSharedPointer<GPUFilterMaterial>(pMaterial));

    QMatrix4x4 mat;
    QVector3D pos = m_floorPostion;
    pos.setX(m_floorPostion.x() + 7.5f);
    pos.setY(m_floorPostion.y() + 1.0f);
    pos.setZ(m_floorPostion.z() + 2.5f);
    mat.translate(pos);
    mat.scale(1.0f, -1.0f, 1.0f);
    pMesh->setModelMartix(mat);
}

void GPUFilterVideoPlayerScene::createTestLights(void)
{
    // Add Light
    GPUFilterDirectionLight* pLight = new GPUFilterDirectionLight;
    pLight->setDirection(QVector3D(-1.0f, -1.0f, -1.0f));
    this->addLight(pLight);

#if 1
    QVector<QVector3D> posVec;
    posVec << QVector3D(m_floorPostion.x() + 8.0f, m_floorPostion.y() + 4.0f, m_floorPostion.z() + 8.0f)
           << QVector3D(m_floorPostion.x() - 8.0f, m_floorPostion.y() + 4.0f, m_floorPostion.z() + 8.0f)
           << QVector3D(m_floorPostion.x() + 8.0f, m_floorPostion.y() + 4.0f, m_floorPostion.z() - 8.0f)
           << QVector3D(m_floorPostion.x() - 8.0f, m_floorPostion.y() + 4.0f, m_floorPostion.z() - 8.0f);

    for (auto iter = posVec.begin(); iter != posVec.end(); ++iter)
    {
        QVector3D pointPos = *iter;

        // Add Point Light
        GPUFilterPointLight* pPointLight = new GPUFilterPointLight;
        pPointLight->setLightPostion(pointPos);
        this->addLight(pPointLight);

        // Add Test Light Postion
        GPUFilterGeometry* pMesh = new GPUFilterGeometry;
        this->addMesh(pMesh);
        GPUFilterMaterial* pMaterial = new GPUFilterMaterial;
        pMaterial->setAmbientColor(QVector3D(1.0f, 1.0f, 1.0f));
        pMaterial->setDiffuesColor(QVector3D(1.0f, 1.0f, 1.0f));
        pMaterial->setSpecularColor(QVector3D(1.0f, 1.0f, 1.0f));
        pMaterial->setColorEnabled(true);
        pMaterial->setLightEffect(false);
        pMesh->setMaterial(QSharedPointer<GPUFilterMaterial>(pMaterial));
        pMesh->setVisible(true);

        QMatrix4x4 mat;
        mat.translate(pointPos);
        mat.scale(0.05f, 0.05f, 0.05f);
        pMesh->setModelMartix(mat);
    }

#endif
    // Add Spot Light
    GPUFilterSpotLight* pSpotLight = new GPUFilterSpotLight;
    QVector3D spotPoint = m_floorPostion;
    spotPoint.setY(spotPoint.x() + 10.0f);
    spotPoint.setY(spotPoint.y() + 1.0f);
    spotPoint.setZ(spotPoint.z() + 5.0f);
    pSpotLight->setLightPostion(spotPoint);
    pSpotLight->setDirection(QVector3D(0.0f, -1.0f, 0.0f));
    pSpotLight->setCutoutInfo(60.0f, 65.0f);
    this->addLight(pSpotLight);

    GPUFilterGeometry* pMesh = new GPUFilterGeometry;
    this->addMesh(pMesh);
    GPUFilterMaterial* pMaterial = new GPUFilterMaterial;
    pMaterial->setAmbientColor(QVector3D(1.0f, 1.0f, 1.0f));
    pMaterial->setDiffuesColor(QVector3D(1.0f, 1.0f, 1.0f));
    pMaterial->setSpecularColor(QVector3D(1.0f, 1.0f, 1.0f));
    pMaterial->setColorEnabled(true);
    pMaterial->setLightEffect(false);
    pMesh->setMaterial(QSharedPointer<GPUFilterMaterial>(pMaterial));
    pMesh->setVisible(true);

    QMatrix4x4 mat;
    mat.translate(spotPoint);
    mat.scale(0.05f, 0.05f, 0.05f);
    pMesh->setModelMartix(mat);

#if 0
    // Add Flash Light
    GPUFilterFlashLight* pFlashLight = new GPUFilterFlashLight;
    pFlashLight->setDiffuesColor(QVector3D(200.0 / 255 * 0.5f, 87.0 / 255 * 0.5f, 217.0 / 255 * 0.5f));
    pFlashLight->setSpecularColor(QVector3D(0.0f, 0.0f, 0.0f));
    pFlashLight->setCutoutInfo(1.2f, 4.0f);
    this->addLight(pFlashLight);
#endif
}

void GPUFilterVideoPlayerScene::createFloor(void)
{
    m_pFloorMesh = new GPUFilterFloorMesh;
    this->addMesh(m_pFloorMesh);

    // Set Floor Material
    GPUFilterMaterial* pMaterial = new GPUFilterMaterial;
    pMaterial->setColorEnabled(true);
    pMaterial->setExtraTextureEnabled(false);
    pMaterial->setLightEffect(true);
    m_pFloorMesh->setMaterial(QSharedPointer<GPUFilterMaterial>(pMaterial));
    pMaterial->setAmbientColor(QVector3D(0.78f, 0.78f, 0.78f));
    pMaterial->setDiffuesColor(QVector3D(0.78f, 0.78f, 0.78f));
    pMaterial->setSpecularColor(QVector3D(0.78f, 0.78f, 0.78f));

    QMatrix4x4 mat;
    QVector3D pointPos(0.0f, -5.0f, -20.0f);
    m_floorPostion = pointPos;
    mat.translate(m_floorPostion);
    mat.scale(10.0f, 10.0f, 10.0f);
    mat.rotate(90.0f, QVector3D(1.0f, 0.0f, 0.0f));
    m_pFloorMesh->setModelMartix(mat);
}

void GPUFilterVideoPlayerScene::createTVMesh(void)
{
    m_pTVMesh = new GPUFilterGeometryRect;
    this->addMesh(m_pTVMesh);

    // Set Floor Material
#if 0
    m_pMaterial = new GPUFilterMaterial;
    m_pMaterial->setColorEnabled(true);
    m_pMaterial->setExtraTextureEnabled(false);
    m_pMaterial->setLightEffect(false);
    m_pMaterial->setAmbientColor(QVector3D(0.0f, 0.0f, 0.0f));
    m_pMaterial->setDiffuesColor(QVector3D(0.0f, 0.0f, 0.0f));
    m_pMaterial->setSpecularColor(QVector3D(0.0f, 0.0f, 0.0f));
#endif
    m_pTVMesh->setMaterial(QSharedPointer<GPUFilterMaterial>(m_pMaterial));
    m_pMaterial->setLightEffect(true);

    // Set Mode Matrix
    QMatrix4x4 mat;
    QVector3D pointPos(0.0f, 0.0f, m_floorPostion.z() - 2.5f);
    mat.translate(pointPos);
    mat.scale(5.0f, m_floorPostion.y(), 1.0f);
    m_pTVMesh->setModelMartix(mat);
}

void GPUFilterVideoPlayerScene::createTVMesh2(void)
{
    m_pTVMesh2 = new GPUFilterInvertedMesh;
    this->addMesh(m_pTVMesh2);
    m_pTVMesh2->setMaterial(QSharedPointer<GPUFilterMaterial>(m_pMaterial));

    // Set Mode Matrix
    QMatrix4x4 mat;
    QVector3D pointPos(0.0f, 0.0f, m_floorPostion.z() - 2.5f);
    pointPos.setY(-pointPos.y() + 2 * m_floorPostion.y());
    mat.translate(pointPos);
    mat.scale(5.0f, -m_floorPostion.y(), 1.0f);
    m_pTVMesh2->setModelMartix(mat);
}

void GPUFilterVideoPlayerScene::createModelObject(void)
{
    m_pModel = new GPUFilterModel(this);

    QMatrix4x4 mat;
    QVector3D pos = m_floorPostion;
    pos.setX(m_floorPostion.x() - 5.5f);
    pos.setY(m_floorPostion.y() + 1.0f);
    pos.setZ(m_floorPostion.z() + 4.5f);
    mat.translate(pos);
    mat.scale(4.0f, 4.0f, 4.0f);

    m_pModel->setModelMatrix(mat);
}

void GPUFilterVideoPlayerScene::createAnimationModelObject(void)
{
    m_pAnimationModel = new GPUFilterModel(this);

    QMatrix4x4 mat;
    QVector3D pos = m_floorPostion;
    pos.setX(m_floorPostion.x() - 1.5f);
    pos.setY(m_floorPostion.y() + 1.0f);
    pos.setZ(m_floorPostion.z() + 10.0f);
    mat.translate(pos);
    mat.scale(4.0f, 4.0f, 4.0f);
    m_pAnimationModel->setModelMatrix(mat);
}

void GPUFilterVideoPlayerScene::setYUVData(int type, const QVector<QByteArray>& yuvData, int width, int height)
{
    if (yuvData.size() <= 2)
        return;

    m_pMaterial->setColorEnabled(false);
    m_pMaterial->setExtraTextureEnabled(true);

    // Set Y Data
    if (m_pMaterial->getExtraTexture1().isNull())
    {
        QSharedPointer<GPUFilterTexture> pYTexture(new GPUFilterTexture);
        pYTexture->setImageFormat(GPUFilterTexture::t_LUMINANCE);
        pYTexture->setPBOEnabled(true);
        m_pMaterial->setExtraTexture1(pYTexture);
    }
    m_pMaterial->getExtraTexture1()->setImageData(yuvData[0].data(), width, height);

    // Set U Data
    if (m_pMaterial->getExtraTexture2().isNull())
    {
        QSharedPointer<GPUFilterTexture> pUTexture(new GPUFilterTexture);
        pUTexture->setImageFormat(GPUFilterTexture::t_LUMINANCE);
        pUTexture->setPBOEnabled(true);
//        pUTexture->setImageData(yuvData[1].data(), width / 2, height / 2);
        m_pMaterial->setExtraTexture2(pUTexture);
    }
    m_pMaterial->getExtraTexture2()->setImageData(yuvData[1].data(), width / 2, height / 2);

    // Set V Data
    if (m_pMaterial->getExtraTexture3().isNull())
    {
        QSharedPointer<GPUFilterTexture> pVTexture(new GPUFilterTexture);
        pVTexture->setImageFormat(GPUFilterTexture::t_LUMINANCE);
        pVTexture->setPBOEnabled(true);
//        pVTexture->setImageData(yuvData[2].data(), width / 2, height / 2);
        m_pMaterial->setExtraTexture3(pVTexture);
    }
    m_pMaterial->getExtraTexture3()->setImageData(yuvData[2].data(), width / 2, height / 2);

    // Set Scale Size
    int scaleSize = qMax(width, height);
    float scaleWidth = width * 5.0 / scaleSize;
    float scaleHeight = height * 5.0 / scaleSize;

    QMatrix4x4 mat;
    QVector3D pointPos(0.0f, scaleHeight - 5.0f, m_floorPostion.z() - 1.5f);
    mat.translate(pointPos);
    mat.scale(scaleWidth, -scaleHeight, 5.0f);
    m_pTVMesh->setModelMartix(mat);

    // Set Mode Matrix
    mat.setToIdentity();
    pointPos.setY(-pointPos.y() + 2 * m_floorPostion.y());
    mat.translate(pointPos);
    mat.scale(scaleWidth, scaleHeight, 5.0f);
    m_pTVMesh2->setModelMartix(mat);
}

void GPUFilterVideoPlayerScene::loadAnimationModel(const QString& modelFilePath)
{
    m_pAnimationModel->loadModel(modelFilePath, true);
    m_pAnimationModel->loadAnimation(modelFilePath);
    m_pAnimationModel->addToScene(this);
}

void GPUFilterVideoPlayerScene::loadModel(const QString& modelFilePath)
{
    m_pModel->loadModel(modelFilePath);
    m_pModel->addToScene(this);
}
