#include "widget.h"
#include <QVBoxLayout>
#include <QFileDialog>
#include "GPUFilterVideoDecodec.h"
#include "YUVToRGBProcesser.h"
#include "RGBToYUVProcesser.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    init();

    // Create Video Decodec
    m_pVideoDecodec = new GPUFilterVideoDecodec(this);
    QObject::connect(m_pVideoDecodec, &GPUFilterVideoDecodec::updateDisplay, this, &Widget::onUpdateDisplay);

    m_pYUVToRGBProcesser = new YUVToRGBProcesser(this);
}

Widget::~Widget()
{

}

void Widget::init(void)
{
    QVBoxLayout* mainLayout = new QVBoxLayout(this);

    // Top Widget
    QWidget* pTopWidget = new QWidget;
    mainLayout->addWidget(pTopWidget);
    QHBoxLayout* pTopLayout = new QHBoxLayout(pTopWidget);
    pTopLayout->setMargin(0);
    // Tag Label
    QLabel* pTopLable = new QLabel(tr("YUVToRGB:"));
    pTopLayout->addWidget(pTopLable);
    // File Path
    m_pVideoFilePathLineEdit = new QLineEdit;
    pTopLayout->addWidget(m_pVideoFilePathLineEdit);
    // Browse
    QPushButton* pBroseButton = new QPushButton(tr("Brose"));
    pTopLayout->addWidget(pBroseButton);
    QObject::connect(pBroseButton, &QPushButton::clicked, this, &Widget::onClickedBroseVideoButton);
    // Begin Conver Button
    QPushButton* pBeginConvertButton = new QPushButton(tr("Begin Conver"));
    m_YUVToRGBButton = pBeginConvertButton;
    m_YUVToRGBButton->setEnabled(false);
    pTopLayout->addWidget(pBeginConvertButton);
    QObject::connect(pBeginConvertButton, &QPushButton::clicked, this, &Widget::onClickedConverYUVToRGBButton);

    // Bottom Widget
    QWidget* pBottomWidget = new QWidget;
    mainLayout->addWidget(pBottomWidget);
    // Bottom Tag
    QHBoxLayout* pBottomLaout = new QHBoxLayout(pBottomWidget);
    pBottomLaout->setMargin(0);
    QLabel* pBottomTag = new QLabel(tr("RGBToYUV:"));
    pBottomLaout->addWidget(pBottomTag);
    // File Path
    m_pImagePathLineEdit = new QLineEdit;
    pBottomLaout->addWidget(m_pImagePathLineEdit);
    // Image Button
    QPushButton* pImageButton = new QPushButton(tr("Brose"));
    pBottomLaout->addWidget(pImageButton);
    QObject::connect(pImageButton, &QPushButton::clicked, this, &Widget::onClickedBroseImageButton);
    // Begin Button
    QPushButton* pBeginButton = new QPushButton(tr("ConverToYUVImage"));
    pBottomLaout->addWidget(pBeginButton);
    QObject::connect(pBeginButton, &QPushButton::clicked, this, &Widget::onClickedConverRGBToYUVButton);
}

void Widget::onClickedBroseVideoButton(void)
{
    QString fileName = QFileDialog::getOpenFileName(this, "Open Video", "./");
    if (fileName.isEmpty())
        return;

    m_pVideoFilePathLineEdit->setText(fileName);
    m_YUVToRGBButton->setEnabled(true);
}

void Widget::onClickedConverYUVToRGBButton(void)
{
    m_isRunningConver = !m_isRunningConver;
    if (m_isRunningConver)
        m_YUVToRGBButton->setText(tr("End Conver"));
    else
        m_YUVToRGBButton->setText(tr("Begin Conver"));

    if (m_isYUVToRGBInited)
        return;

    if (m_pVideoDecodec->openVideoFile(m_pVideoFilePathLineEdit->text()))
        m_pVideoDecodec->start();
}

void Widget::onClickedBroseImageButton(void)
{

}

void Widget::onClickedConverRGBToYUVButton(void)
{

}

void Widget::onUpdateDisplay(void)
{
    if (m_pVideoDecodec == nullptr || !m_isRunningConver)
        return;

    QVector<QByteArray> yuvDatas;
    int type = 0;
    m_pVideoDecodec->getYUVData(yuvDatas, type);

    int nWidth, nHeight;
    m_pVideoDecodec->getVideoSize(nWidth, nHeight);

    if (!m_isYUVToRGBInited)
    {
        m_pYUVToRGBProcesser->init();
        m_isYUVToRGBInited = true;
    }

    m_pYUVToRGBProcesser->resize(nWidth, nHeight);

    // Set YUV Data
    m_pYUVToRGBProcesser->setYData(yuvDatas[0], nWidth, nHeight);
    m_pYUVToRGBProcesser->setUData(yuvDatas[1], nWidth, nHeight);
    m_pYUVToRGBProcesser->setVData(yuvDatas[2], nWidth, nHeight);
    // Render
    m_pYUVToRGBProcesser->render();

    // Save Image
    QImage image = m_pYUVToRGBProcesser->packImage();
    static int number = 0;
    QString imageName = "%1/%2.bmp";
    imageName = imageName.arg(qApp->applicationDirPath() + "/ConverImage/").arg(number++);
    qDebug() << imageName;
    image.save(imageName);

    //m_pRenderWidget->setYUVData(type, yuvDatas, nWidth, nHeight);
}

