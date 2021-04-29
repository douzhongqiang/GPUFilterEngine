#ifndef GPUFILTERVIDEODECODEC_H
#define GPUFILTERVIDEODECODEC_H

#include <QObject>
#include <QThread>
#include <atomic>
#include <QSemaphore>
#include "GPUFilterVideoCore_global.h"

extern "C"{
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
#include <libavutil/imgutils.h>
#include <libswresample/swresample.h>
}

class GPUFILTERVIDEOCORE_EXPORT GPUFilterVideoDecodec : public QThread
{
    Q_OBJECT

public:
    GPUFilterVideoDecodec(QObject* parent = nullptr);
    ~GPUFilterVideoDecodec();

    // open video file
    bool openVideoFile(const QString& fileName);

    // get Frame Data
    bool getYUVData(QVector<QByteArray>& yuvData, int& type);

    void run(void) override;

private:
    AVFormatContext* m_pFormatContext = nullptr;
    AVCodecContext* m_pVideoCodecContext = nullptr;

    int m_nVideoIndex = -1;

    // open video decodec
    bool openVideoCodec(void);
    // decodec Video
    bool decodecVideo(AVPacket* packet);

    int m_nVideoWidth = 0;
    int m_nVideoHeight = 0;
    enum AVPixelFormat m_pixelFormat;

    QSemaphore m_semaphore;

    const int m_nTotal = 5;
    std::atomic<int> m_nStartIndex;
    std::atomic<int> m_nEndIndex;
    QVector<AVFrame*> m_frames;
};

#endif
