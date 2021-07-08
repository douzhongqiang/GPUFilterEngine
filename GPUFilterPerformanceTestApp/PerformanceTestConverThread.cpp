#include "PerformanceTestConverThread.h"
#include <QTime>
#include <QDebug>
#include <QApplication>

PerformanceTestConverThread::PerformanceTestConverThread(QObject* parent)
    :QThread(parent)
    , m_isUsedGPU(true)
    , m_isResizeEnabled(false)
    , m_nResizeWidth(0)
    , m_nResizeHeight(0)
{

}

PerformanceTestConverThread::~PerformanceTestConverThread()
{
    this->requestInterruption();
    m_waitCondition.wakeAll();
    this->wait();

    if (m_pVideoEncodec)
    {
        m_pVideoEncodec->endVideoEncodec();
        m_pVideoEncodec->deleteLater();
    }
}

void PerformanceTestConverThread::addImage(const QImage& image)
{
    QMutexLocker locker(&m_mutex);
    m_imageList.push_back(image);
    m_waitCondition.wakeAll();
}

void PerformanceTestConverThread::run(void)
{
    while (!this->isInterruptionRequested())
    {
        m_mutex.lock();
        while (m_imageList.isEmpty() && !this->isInterruptionRequested())
            m_waitCondition.wait(&m_mutex);

        QList<QImage> tempImageList = m_imageList;
        m_imageList.clear();
        m_mutex.unlock();

        if (m_pConverProcesser == nullptr)
        {
            m_pConverProcesser = new RGBToYUVProcesser;
            m_pConverProcesser->init();
        }

        if (m_pVideoEncodec == nullptr)
        {
            m_pVideoEncodec = new GPUFilterVideoEncodec;
            QString str = qApp->applicationDirPath() + QString("/ConverImage2/") + "Test.mp4";
            GPUFilterVideoEncodec::VideoInfo info;
            info.width = tempImageList.at(0).width();
            info.height = tempImageList.at(0).height();
            m_pVideoEncodec->setCreateVideoInfo(str, info);
            m_pVideoEncodec->startVideoEncodec();
        }

        for (auto iter = tempImageList.begin(); iter != tempImageList.end(); ++iter)
        {

            // Conver By GPU
            QImage image = *iter;
#if 0
            // GPU
            qDebug() << "================== Begin Conver ==================================================";
            static int number = 0;
            int delayValue = 0;

            QTime time;
            time.start();
            // Begin Conver
            if (m_isUsedGPU)
            {
                int nWidth = image.width() / 4;
                int nHeight = image.height() + image.height() / 2;

                if (m_isResizeEnabled)
                {
                    nWidth = m_nResizeWidth / 4;
                    nHeight = m_nResizeHeight + m_nResizeHeight / 2;
                }

                m_pConverProcesser->resize(nWidth, nHeight);
                int n1 = time.elapsed();
                m_pConverProcesser->setImage(image);
                int n2 = time.elapsed();
                m_pConverProcesser->render();
                int n3 = time.elapsed();
                QImage tempImage = m_pConverProcesser->packImage();
                // End Conver
                qDebug() << "Conver To YUV Data Frame:" << number++ << "[" << n1 << n2 << n3 << time.elapsed() << "]";

//                QString imagePath = QString("%1/ConverImage2/%2.bmp").arg(qApp->applicationDirPath()).arg(number);
//                tempImage.save(imagePath);
            }
            else
            {
                // FFMpeg
                m_pVideoEncodec->rgbConverToYUV(image);
                qDebug() << "Conver To YUV Data Frame:" << number++ << time.elapsed();
            }            
            delayValue += time.elapsed();
            m_nTotal += delayValue;
            m_avgValue = m_nTotal * 1.0 / number;
            emit sendValues(delayValue, m_avgValue);
#endif
            m_pVideoEncodec->addImage(image);
        }
    }
}

void PerformanceTestConverThread::startConver(void)
{
    this->start();
}

void PerformanceTestConverThread::setUsedGPU(bool isUsedGPU)
{
    m_isUsedGPU = isUsedGPU;
}

bool PerformanceTestConverThread::isUsedGPU(void)
{
    return m_isUsedGPU;
}

void PerformanceTestConverThread::setResizeEnabled(bool isEnabled)
{
    m_isResizeEnabled = isEnabled;
    m_pConverProcesser->setRenderType(!m_isResizeEnabled);
}

bool PerformanceTestConverThread::isResizeEnabled(void)
{
    return m_isResizeEnabled;
}

void PerformanceTestConverThread::setResizeSize(int width, int height)
{
    m_nResizeWidth = width;
    m_nResizeHeight = height;
}
