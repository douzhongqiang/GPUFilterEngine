#ifndef GPUFILTERVIDEOENCODEC_H
#define GPUFILTERVIDEOENCODEC_H

#include <QObject>
#include <QImage>
#include <QWaitCondition>
#include <QMutex>
#include <QThread>
#include <QTimer>
#include "GPUFilterVideoCore_global.h"
extern "C"{
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
#include <libavutil/imgutils.h>
#include <libswresample/swresample.h>
#include <libavutil/opt.h>
}

class GPUFILTERVIDEOCORE_EXPORT GPUFilterVideoEncodec : public QThread
{
    Q_OBJECT

public:
    struct VideoInfo
    {
        int width = 800;
        int height = 600;

        int fts = 30;
        int coverType = 0;      // YUV420P
    };

public:
    GPUFilterVideoEncodec(QObject* parent = nullptr);
    ~GPUFilterVideoEncodec();

    void setCreateVideoInfo(const QString& fileName, const VideoInfo& videoInfo);

    void addImage(const QImage& image);
    bool startVideoEncodec(void);
    void endVideoEncodec(void);

    void run(void) override;

private:
    AVCodec *m_pVideoCodec = nullptr;
    AVCodecContext *m_pVideoCodecContext = nullptr;
    AVFormatContext *m_pFormatContext = nullptr;
    AVOutputFormat *m_pOutputFormat = nullptr;

    AVFrame* m_pFrame = nullptr;
    AVFrame* m_pTempFrame = nullptr;
    SwsContext* m_pSwsContext = nullptr;

    QString m_fileName;
    VideoInfo m_createInfo;
    int m_nPtsIndex = 0;

    bool createFormat(void);
    void releaseAll(void);

    bool rgbConverToYUV(void);

private:
    QWaitCondition m_condition;
    QMutex m_mutex;
    QList<QImage> m_imageList;

    void writeImage(const QImage& image);

    void initTimer(void);
    void startTimer(int interval);
    void stopTimer(void);
    QTimer* m_pTimer = nullptr;
    QThread* m_pThread = nullptr;

signals:
    void requestInputImage(void);
};

#endif
