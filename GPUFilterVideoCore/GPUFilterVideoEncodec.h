#ifndef GPUFILTERVIDEOENCODEC_H
#define GPUFILTERVIDEOENCODEC_H

#include <QObject>
#include "GPUFilterVideoCore_global.h"
extern "C"{
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
#include <libavutil/imgutils.h>
#include <libswresample/swresample.h>
#include <libavutil/opt.h>
}

class GPUFILTERVIDEOCORE_EXPORT GPUFilterVideoEncodec : public QObject
{
    Q_OBJECT

public:
    struct VideoInfo
    {
        int width = 800;
        int height = 600;

        int fts = 30;
    };

public:
    GPUFilterVideoEncodec(QObject* parent = nullptr);
    ~GPUFilterVideoEncodec();

    void setCreateVideoInfo(const QString& fileName, const VideoInfo& videoInfo);

    bool startVideoEncodec(void);
    void endVideoEncodec(void);

    void writeImage(const QImage& image);

private:
    AVCodec *m_pVideoCodec = nullptr;
    AVCodecContext *m_pVideoCodecContext = nullptr;
    AVFormatContext *m_pFormatContext = nullptr;
    AVOutputFormat *m_pOutputFormat = nullptr;

    QString m_fileName;
    VideoInfo m_createInfo;

    bool createFormat(void);
};

#endif
