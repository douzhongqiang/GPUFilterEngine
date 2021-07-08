#ifndef PERFORMANCETESTCONVERTHREAD_H
#define PERFORMANCETESTCONVERTHREAD_H

#include <QThread>
#include <QObject>
#include <QWaitCondition>
#include <QMutex>
#include <atomic>
#include "RGBToYUVProcesser.h"
#include "GPUFilterVideoEncodec.h"

class PerformanceTestConverThread : public QThread
{
    Q_OBJECT

public:
    PerformanceTestConverThread(QObject* parent = nullptr);
    ~PerformanceTestConverThread();

    void run(void) override;

    void startConver(void);

    void addImage(const QImage& image);

    void setUsedGPU(bool isUsedGPU);
    bool isUsedGPU(void);

    void setResizeEnabled(bool isEnabled);
    bool isResizeEnabled(void);
    void setResizeSize(int width, int height);

private:
    RGBToYUVProcesser* m_pConverProcesser = nullptr;
    GPUFilterVideoEncodec* m_pVideoEncodec = nullptr;

    QList<QImage> m_imageList;
    bool m_isInited = false;

    QMutex m_mutex;
    QWaitCondition m_waitCondition;
    std::atomic<bool> m_isUsedGPU;

    qint64 m_nTotal = 0;
    qint64 m_avgValue = 0;

    std::atomic<bool> m_isResizeEnabled;
    std::atomic<int> m_nResizeWidth;
    std::atomic<int> m_nResizeHeight;

signals:
    void sendValues(qint64 currentValue, qint64 avgValue);

private slots:
    void onRecvDeleay(int time);
};

#endif
