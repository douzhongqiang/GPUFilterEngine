#include "widget.h"
#include <QVBoxLayout>
#include <QPushButton>
#include <QThread>
#include <QFileDialog>
#include <QMatrix4x4>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    m_pRenderWidget = new GPUFilterVideoPlayerWidget;
    m_pRenderWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    QVBoxLayout* pLayout = new QVBoxLayout(this);
    pLayout->addWidget(m_pRenderWidget);

    // Add Video Widget
    pLayout->addWidget(createVideoWidget());
    // Add Model Widget
    pLayout->addWidget(createModelWidget());
    
    // Create Video Decodec
    m_pVideoDecodec = new GPUFilterVideoDecodec(this);
    QObject::connect(m_pVideoDecodec, &GPUFilterVideoDecodec::updateDisplay, this, &Widget::onUpdateDisplay);

    // Create Video Encodec
    m_pVideoEncodec = new GPUFilterVideoEncodec(this);
    QObject::connect(m_pVideoEncodec, &GPUFilterVideoEncodec::requestInputImage, this, &Widget::onRecordTimeout);
}

Widget::~Widget()
{
    
}

QWidget* Widget::createVideoWidget(void)
{
    QWidget* pVideoControlWidget = new QWidget;
    QHBoxLayout* pLayout = new QHBoxLayout(pVideoControlWidget);
    pLayout->setSpacing(8);
    pLayout->setMargin(0);

    //
    QPushButton* pButtonOpenVideo = new QPushButton("Load Video");
    pLayout->addWidget(pButtonOpenVideo);
    QObject::connect(pButtonOpenVideo, &QPushButton::clicked, this, &Widget::onClickedButton);

    m_pRecordVideoButton = new QPushButton("Record Video");
    pLayout->addWidget(m_pRecordVideoButton);
    QObject::connect(m_pRecordVideoButton, &QPushButton::clicked, this, &Widget::onClickedRecordButton);

    return pVideoControlWidget;
}

QWidget* Widget::createModelWidget(void)
{
    QWidget* pW = new QWidget;
    QHBoxLayout* pLayout = new QHBoxLayout(pW);
    pLayout->setSpacing(8);
    pLayout->setMargin(0);

    QPushButton* pLoadModelButton = new QPushButton("Load Model");
    pLayout->addWidget(pLoadModelButton);
    QObject::connect(pLoadModelButton, &QPushButton::clicked, this, &Widget::onClickedLoadModelButton);

    QPushButton* pLoadAnimationModelButton = new QPushButton("Load Animation Model");
    pLayout->addWidget(pLoadAnimationModelButton);
    QObject::connect(pLoadAnimationModelButton, &QPushButton::clicked, this, &Widget::onClickedLoadAnimationModelButton);

    return pW;
}

void Widget::onClickedButton(void)
{
    if (m_pVideoDecodec && !m_pVideoDecodec->isRunning())
    {
        QString filename = QFileDialog::getOpenFileName(this, "Open Video", "./");
        if (filename.isEmpty())
            return;

        if (m_pVideoDecodec->openVideoFile(filename))
            m_pVideoDecodec->start();
    }
}

void Widget::onClickedRecordButton(void)
{
    QString fileName = qApp->applicationDirPath() + "/TestVideo.mp4";
    m_nWidth = (m_pRenderWidget->width() >> 4) << 4;
    m_nHeight = (m_pRenderWidget->height() >> 4) << 4;

    if (m_isRecording)
    {
        m_pVideoEncodec->endVideoEncodec();
    }
    else
    {
        GPUFilterVideoEncodec::VideoInfo videoInfo;
        videoInfo.width = m_nWidth;
        videoInfo.height = m_nHeight;

        m_pVideoEncodec->setCreateVideoInfo(fileName, videoInfo);
        m_pVideoEncodec->startVideoEncodec();
    }

    m_isRecording = !m_isRecording;

    if (m_isRecording)
    {
        m_pRecordVideoButton->setText("Recording");
    }
    else
    {
        m_pRecordVideoButton->setText("Record Video");
    }
}

void Widget::onUpdateDisplay(void)
{
    if (m_pVideoDecodec == nullptr)
        return;

    QVector<QByteArray> yuvDatas;
    int type = 0;
    m_pVideoDecodec->getYUVData(yuvDatas, type);

    int nWidth, nHeight;
    m_pVideoDecodec->getVideoSize(nWidth, nHeight);

    m_pRenderWidget->setYUVData(type, yuvDatas, nWidth, nHeight);
}

void Widget::onClickedLoadModelButton(void)
{
    QString filename = QFileDialog::getOpenFileName(this, "Open Model", "./");
    if (filename.isEmpty())
        return;

    m_pRenderWidget->loadModel(filename);
}

void Widget::onClickedLoadAnimationModelButton(void)
{
    QString filename = QFileDialog::getOpenFileName(this, "Open Animation Model", "./");
    if (filename.isEmpty())
        return;

    m_pRenderWidget->loadAnimationModel(filename);
}

void Widget::onRecordTimeout(void)
{
    QTime time;
    time.start();
    static int nFrame = 0;
#if 0
    QPixmap pixmap = m_pRenderWidget->grab(QRect(0, 0, m_nWidth, m_nHeight));
    QImage image = pixmap.toImage();
    image.convertTo(QImage::Format_RGB888);

    qDebug() << "Grab Image Delay" << time.elapsed() << ", " << nFrame++;
#else
    QImage tempImage = m_pRenderWidget->grapImage(m_nWidth, m_nHeight);
    if (!tempImage.isNull())
    {
        tempImage = tempImage.mirrored();
    }
    qDebug() << "Grab Image Delay" << time.elapsed() << ", " << nFrame++;
#endif

    m_pVideoEncodec->addImage(tempImage);
}
