#include "widget.h"
#include <QVBoxLayout>
#include <QPushButton>
#include <QThread>
#include <QFileDialog>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    m_pRenderWidget = new GPUFilterVideoPlayerWidget;
    m_pRenderWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    QVBoxLayout* pLayout = new QVBoxLayout(this);
    pLayout->addWidget(m_pRenderWidget);

    QPushButton* pButton = new QPushButton("Load");
    pLayout->addWidget(pButton);
    QObject::connect(pButton, &QPushButton::clicked, this, &Widget::onClickedButton);

    // Create Video Decodec
    m_pVideoDecodec = new GPUFilterVideoDecodec(this);
    QObject::connect(m_pVideoDecodec, &GPUFilterVideoDecodec::updateDisplay, this, &Widget::onUpdateDisplay);
}

Widget::~Widget()
{

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
