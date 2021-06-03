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
#include "GPUFilterModel.h"

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

    g_GPUFunc->glEnable(GL_DEPTH_TEST);
    g_GPUFunc->glDepthFunc(GL_LESS);

    g_GPUFunc->glEnable(GL_STENCIL_TEST);
    g_GPUFunc->glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
    g_GPUFunc->glStencilFunc(GL_ALWAYS, 1, 0xFF);

    g_GPUFunc->glClearColor(m_bgColor.redF(), m_bgColor.greenF(), m_bgColor.blueF(), m_bgColor.alphaF());
    g_GPUFunc->glClearStencil(0);
    g_GPUFunc->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    m_pShaderProgram->bind();

    effectLights();

    g_GPUFunc->glStencilMask(0x00);

    // Draw Mesh
    for (auto iter = m_meshVec.begin(); iter != m_meshVec.end(); ++iter)
    {
        GPUFilterMesh* pMesh = *iter;
        if (pMesh == nullptr)
            continue;

        // Set View And Projection Matrix
        m_pShaderProgram->getShaderProgram()->setUniformValue("V", m_pCamera->getVMatrix());
        m_pShaderProgram->getShaderProgram()->setUniformValue("P", m_pCamera->getPMatrix());
        // Set Camera Postion
        m_pShaderProgram->getShaderProgram()->setUniformValue("ViewPostion", m_pCamera->getCameraPostion());

        if (pMesh == m_pFloorMesh)
        {
            g_GPUFunc->glStencilFunc(GL_ALWAYS, 1, 0xFF);
            g_GPUFunc->glStencilMask(0xFF);
            g_GPUFunc->glDepthMask(GL_FALSE);
        }
        else if (pMesh == m_pTVMesh2)
        {
            g_GPUFunc->glStencilFunc(GL_EQUAL, 1, 0xFF);
            g_GPUFunc->glStencilMask(0x00);

            pMesh->getMaterial()->setFactor(QVector3D(0.4f, 0.4f, 0.4f));
        }
        // Draw
        pMesh->draw();

        if (pMesh == m_pFloorMesh)
        {
            g_GPUFunc->glDepthMask(GL_TRUE);
        }
        else if (pMesh == m_pTVMesh2)
        {
            g_GPUFunc->glStencilMask(0xFF);
            pMesh->getMaterial()->setFactor(QVector3D(1.0f, 1.0f, 1.0f));
        }
    }

    m_pShaderProgram->unbind();
}

void GPUFilterVideoPlayerScene::initScene(void)
{
    createTestRectMesh();
    createTestCubeMesh();
    createTestLights();

    createTVMesh();
    createFloor();
    createTVMesh2();

    createModelObject();
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

    QImage image("./green-5919790_1280.jpg");
    QSharedPointer<GPUFilterTexture> pTexture(new GPUFilterTexture(this));
    pTexture->setImage(image);
    pTexture->setObjectName("Texture1");

    QImage image2("./city-4478471_1280.jpg");
    QSharedPointer<GPUFilterTexture> pTexture2(new GPUFilterTexture(this));
    pTexture2->setImage(image2);
    pTexture2->setObjectName("Texture2");

    QImage image3("./digital-paper-4906487_1280.jpg");
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

    // Set Texture
    QSharedPointer<GPUFilterTexture> pTexture1(new GPUFilterTexture(this));
    pTexture1->setImage(QImage("./container2.png"));
    pMaterial->setAmbientTexture(pTexture1);
    pMaterial->setDiffuesTexture(pTexture1);
    QSharedPointer<GPUFilterTexture> pTexture2(new GPUFilterTexture(this));
    pTexture2->setImage(QImage("./container2_specular.png"));
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

#if 1
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
    m_pFloorMesh = new GPUFilterGeometryRect;
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
    m_pTVMesh2 = new GPUFilterGeometryRect;
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
    pos.setX(m_floorPostion.x() - 7.5f);
    pos.setY(m_floorPostion.y() + 1.0f);
    pos.setZ(m_floorPostion.z() + 2.5f);
//    mat.rotate(-90.0f, 1.0f, 0.0f, 0.0f);
//    mat.scale(0.1f, 0.1f, 0.1f);
    mat.translate(pos);
    m_pModel->setModelMatrix(mat);
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
        m_pMaterial->setExtraTexture1(pYTexture);
    }
    m_pMaterial->getExtraTexture1()->setImageData(yuvData[0].data(), width, height);

    // Set U Data
    if (m_pMaterial->getExtraTexture2().isNull())
    {
        QSharedPointer<GPUFilterTexture> pUTexture(new GPUFilterTexture);
        pUTexture->setImageFormat(GPUFilterTexture::t_LUMINANCE);
//        pUTexture->setImageData(yuvData[1].data(), width / 2, height / 2);
        m_pMaterial->setExtraTexture2(pUTexture);
    }
    m_pMaterial->getExtraTexture2()->setImageData(yuvData[1].data(), width / 2, height / 2);

    // Set V Data
    if (m_pMaterial->getExtraTexture3().isNull())
    {
        QSharedPointer<GPUFilterTexture> pVTexture(new GPUFilterTexture);
        pVTexture->setImageFormat(GPUFilterTexture::t_LUMINANCE);
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

void GPUFilterVideoPlayerScene::loadModel(const QString& modelFilePath)
{
    m_pModel->loadModel(modelFilePath);
    m_pModel->addToScene(this);
}
