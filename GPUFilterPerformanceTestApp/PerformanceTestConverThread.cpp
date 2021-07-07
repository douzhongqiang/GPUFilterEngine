#include "PerformanceTestConverThread.h"
#include <QTime>
#include <QDebug>
#include <QApplication>

PerformanceTestConverThread::PerformanceTestConverThread(QObject* parent)
    :QThread(parent)
    , m_isUsedGPU(true)
{

}

PerformanceTestConverThread::~PerformanceTestConverThread()
{
    this->requestInterruption();
    m_waitCondition.wakeAll();
    this->wait();
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
        while (m_imageList.isEmpty())
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
            m_pVideoEncodec = new GPUFilterVideoEncodec;

        for (auto iter = tempImageList.begin(); iter != tempImageList.end(); ++iter)
        {
            // Conver By GPU
            QImage image = *iter;

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
                m_pConverProcesser->resize(nWidth, nHeight);
                int n1 = time.elapsed();
                m_pConverProcesser->setImage(image);
                int n2 = time.elapsed();
                m_pConverProcesser->render();
                int n3 = time.elapsed();
                QImage tempImage = m_pConverProcesser->packImage();
                // End Conver
                qDebug() << "Conver To YUV Data Frame:" << number++ << "[" << n1 << n2 << n3 << time.elapsed() << "]";
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
