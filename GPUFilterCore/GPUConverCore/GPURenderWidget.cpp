#include "GPURenderWidget.h"
#include "3DExtras/GPUFilterGeometryRect.h"
#include "3DExtras/GPUFilterGeometryCubeBox.h"
#include "OpenGLCore/GPUFilterMaterial.h"
#include "OpenGLCore/GPUFilterTexture.h"
#include "OpenGLCore/GPUFilterDirectionLight.h"
#include "OpenGLCore/GPUFilterPointLight.h"
#include "OpenGLCore/GPUFilterSpotLight.h"
#include "OpenGLCore/GPUFilterFlashLight.h"
#include "OpenGLCore/GPUFilterTool.h"
#include <QSharedDataPointer>
#include <QFileDialog>

GPURenderWidget::GPURenderWidget(QWidget* parent)
    :QOpenGLWidget(parent)
{
    this->setFocusPolicy(Qt::ClickFocus);
    this->setMouseTracking(true);

    m_pMainScene = new GPUFilterScene(this);
    createTestRectMesh();
    //createTestCubeMesh();
    createTestLights();

    // Create Floor
    createFloor();
    // Create TV Mesh
    createTVMesh();
    createTVMesh2();

    m_pConverScene = new GPUConverScene(this);
}

GPURenderWidget::~GPURenderWidget()
{

}

void GPURenderWidget::createTestRectMesh(void)
{
    GPUFilterGeometryRect* pRectMesh = new GPUFilterGeometryRect;
    m_pMainScene->addMesh(pRectMesh);
    m_pMaterial = new GPUFilterMaterial;
    m_pMaterial->setAmbientColor(QVector3D(0.0f, 1.0f, 1.0f));
    m_pMaterial->setDiffuesColor(QVector3D(0.0f, 1.0f, 1.0f));
    m_pMaterial->setSpecularColor(QVector3D(0.0f, 1.0f, 1.0f));
    pRectMesh->setMaterial(QSharedPointer<GPUFilterMaterial>(m_pMaterial));
    pRectMesh->setAlwayToBottom(true);
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

void GPURenderWidget::createTestCubeMesh(void)
{
    GPUFilterGeometry* pMesh = new GPUFilterGeometry;
    m_pMainScene->addMesh(pMesh);
    GPUFilterMaterial* pMaterial = new GPUFilterMaterial;
    pMaterial->setAmbientColor(QVector3D(0.0f, 1.0f, 1.0f));
    pMaterial->setDiffuesColor(QVector3D(0.0f, 1.0f, 1.0f));
    pMaterial->setSpecularColor(QVector3D(0.0f, 1.0f, 1.0f));
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
    mat.translate(0.0f, 0.0f, -1.0f);
    mat.scale(1.0f, -1.0f, 1.0f);
    pMesh->setModelMartix(mat);
}

void GPURenderWidget::createTestLights(void)
{
    // Add Light
    GPUFilterDirectionLight* pLight = new GPUFilterDirectionLight;
    pLight->setDirection(QVector3D(-1.0f, -1.0f, -1.0f));
//    m_pMainScene->addLight(pLight);

    QVector3D pointPos(1.1f, 0.0f, 0.5f);

    // Add Point Light
    GPUFilterPointLight* pPointLight = new GPUFilterPointLight;
    pPointLight->setLightPostion(pointPos);
//    pPointLight->setAmbientColor(QVector3D(1.0f, 1.0f, 1.0f));
//    m_pMainScene->addLight(pPointLight);

    // Add Spot Light
    GPUFilterSpotLight* pSpotLight = new GPUFilterSpotLight;
    pSpotLight->setLightPostion(QVector3D(0.0f, 0.0f, 5.0f));
    pSpotLight->setCutoutInfo(4.0f, 4.5f);
    //m_pMainScene->addLight(pSpotLight);

    // Add Flash Light
    GPUFilterFlashLight* pFlashLight = new GPUFilterFlashLight;
    m_pMainScene->addLight(pFlashLight);

    // Add Test Light Postion
    GPUFilterGeometry* pMesh = new GPUFilterGeometry;
    m_pMainScene->addMesh(pMesh);
    GPUFilterMaterial* pMaterial = new GPUFilterMaterial;
    pMaterial->setAmbientColor(QVector3D(1.0f, 1.0f, 1.0f));
    pMaterial->setDiffuesColor(QVector3D(1.0f, 1.0f, 1.0f));
    pMaterial->setSpecularColor(QVector3D(1.0f, 1.0f, 1.0f));
    pMaterial->setColorEnabled(true);
    pMaterial->setLightEffect(false);
    pMesh->setMaterial(QSharedPointer<GPUFilterMaterial>(pMaterial));
    pMesh->setVisible(false);

    QMatrix4x4 mat;
    mat.translate(pointPos);
    mat.scale(0.05f, 0.05f, 0.05f);
    pMesh->setModelMartix(mat);
}

void GPURenderWidget::createFloor(void)
{
    GPUFilterGeometryRect* pFloorMesh = new GPUFilterGeometryRect;
    m_pMainScene->addMesh(pFloorMesh);

    // Set Floor Material
    GPUFilterMaterial* pMaterial = new GPUFilterMaterial;
    pMaterial->setColorEnabled(false);
    pMaterial->setExtraTextureEnabled(false);
    pMaterial->setLightEffect(false);
    pFloorMesh->setMaterial(QSharedPointer<GPUFilterMaterial>(pMaterial));

    GPUFilterTexture* pTexture = new GPUFilterTexture;
    pTexture->setImage(QImage("./metal.png"));
    pMaterial->setDiffuesTexture(QSharedPointer<GPUFilterTexture>(pTexture));

    QMatrix4x4 mat;
    QVector3D pointPos(0.0f, -5.0f, -20.0f);
    m_floorPostion = pointPos;
    mat.translate(m_floorPostion);
    mat.scale(10.0f, 10.0f, 10.0f);
    mat.rotate(90.0f, QVector3D(1.0f, 0.0f, 0.0f));
    pFloorMesh->setModelMartix(mat);
}

void GPURenderWidget::createTVMesh(void)
{
    m_pTVMesh = new GPUFilterGeometryRect;
    m_pMainScene->addMesh(m_pTVMesh);

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

    // Set Mode Matrix
    QMatrix4x4 mat;
    QVector3D pointPos(0.0f, 0.0f, m_floorPostion.z() - 2.5f);
    mat.translate(pointPos);
    mat.scale(5.0f, m_floorPostion.y(), 1.0f);
    m_pTVMesh->setModelMartix(mat);
}

void GPURenderWidget::createTVMesh2(void)
{
    m_pTVMesh2 = new GPUFilterGeometryRect;
    m_pMainScene->addMesh(m_pTVMesh2);
    m_pTVMesh2->setMaterial(QSharedPointer<GPUFilterMaterial>(m_pMaterial));

    // Set Mode Matrix
    QMatrix4x4 mat;
    QVector3D pointPos(0.0f, 0.0f, m_floorPostion.z() - 2.5f);
    pointPos.setY(-pointPos.y() + 2 * m_floorPostion.y());
    mat.translate(pointPos);
    mat.scale(5.0f, -m_floorPostion.y(), 1.0f);
    m_pTVMesh2->setModelMartix(mat);
}

void GPURenderWidget::setImage(const QImage& image)
{
    this->makeCurrent();
    QString str = QFileDialog::getOpenFileName(this, "Open Image", "./");
    if (str.isEmpty())
        return;

    QSharedPointer<GPUFilterTexture> pTexture(new GPUFilterTexture(this));
    if (pTexture.isNull())
        return;

    pTexture->setImage(QImage(str));
    m_pMaterial->setDiffuesTexture(pTexture);

    this->update();

//#if 1
//    QImage tempImage = image.convertToFormat(QImage::Format_RGB888);
//    m_pMaterial->setColorEnabled(false);
//    m_pTexture = new GPUFilterTexture(this);
//    m_pTexture->create();
//    m_pTexture->setImage(tempImage);
//    m_pMaterial->setAmbientTexture(m_pTexture);
//    m_pMaterial->setDiffuesTexture(m_pTexture);

//    this->update();
//    return;

//#else

//    tempImage = image.convertToFormat(QImage::Format_RGB888);
//    char* pImageData[1] = {0};
//    pImageData[0] = (char*)tempImage.constBits();
//    m_pConverScene->setSrcData(GPUConverScene::t_RGB, (const char**)pImageData, tempImage.width(), tempImage.height());

//    this->update();
//#endif
}

void GPURenderWidget::initializeGL()
{
    this->initializeOpenGLFunctions();
    return m_pMainScene->init();

    m_pConverScene->init();
}

void GPURenderWidget::resizeGL(int w, int h)
{
//    m_pConverScene->resize(w, h);
    m_pMainScene->resize(w, h);

    return QOpenGLWidget::resizeGL(w, h);
}

void GPURenderWidget::paintGL()
{
//    m_pConverScene->render();

//    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    m_pMainScene->render();
}

void GPURenderWidget::keyPressEvent(QKeyEvent* event)
{
    if (m_pMainScene && m_pMainScene->getCamera())
    {
        m_pMainScene->getCamera()->keyPressEvent(event);
        m_pMainScene->getCamera()->activeCamera();
    }

    this->update();
    return QOpenGLWidget::keyPressEvent(event);
}

void GPURenderWidget::mousePressEvent(QMouseEvent* event)
{
    if (m_pMainScene && m_pMainScene->getCamera())
    {
        m_pMainScene->getCamera()->mousePressEvent(event);
        m_pMainScene->getCamera()->activeCamera();
    }

    return QOpenGLWidget::mousePressEvent(event);
}

void GPURenderWidget::mouseMoveEvent(QMouseEvent* event)
{
    if (m_pMainScene && m_pMainScene->getCamera())
    {
        m_pMainScene->getCamera()->mouseMoveEvent(event);
        m_pMainScene->getCamera()->activeCamera();
    }
    this->update();

    return QOpenGLWidget::mouseMoveEvent(event);
}

void GPURenderWidget::mouseReleaseEvent(QMouseEvent* event)
{
    if (m_pMainScene && m_pMainScene->getCamera())
    {
        m_pMainScene->getCamera()->mouseReleaseEvent(event);
        m_pMainScene->getCamera()->activeCamera();
    }

    return QOpenGLWidget::mouseReleaseEvent(event);
}

void GPURenderWidget::wheelEvent(QWheelEvent *event)
{
    if (m_pMainScene && m_pMainScene->getCamera())
    {
        m_pMainScene->getCamera()->wheelEvent(event);
        m_pMainScene->getCamera()->activeCamera();
    }

    this->update();
    return QOpenGLWidget::wheelEvent(event);
}

void GPURenderWidget::setYUVData(int type, const QVector<QByteArray>& yuvData, int width, int height)
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

    this->update();
}
