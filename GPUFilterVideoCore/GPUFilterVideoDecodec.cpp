#include "GPUFilterVideoDecodec.h"
#include <QTimer>
#include <QDebug>
#include <QByteArray>
#include <QImage>
#include <QTime>

GPUFilterVideoDecodec::GPUFilterVideoDecodec(QObject* parent)
    :QThread(parent)
    , m_semaphore(5)
    , m_nStartIndex(0)
    , m_nEndIndex(0)
{
    for (int i=0; i<m_nTotal; ++i)
        m_frames.push_back(nullptr);

    initTimer();
}

GPUFilterVideoDecodec::~GPUFilterVideoDecodec()
{
    this->requestInterruption();

    stopTimer();

    m_semaphore.release();
    this->quit();
    this->wait();
}

void GPUFilterVideoDecodec::initTimer(void)
{
    m_pTimer = new QTimer;
    m_pThread = new QThread(this);
    m_pTimer->setTimerType(Qt::PreciseTimer);
    m_pTimer->moveToThread(m_pThread);

    QObject::connect(m_pTimer, &QTimer::timeout, this, &GPUFilterVideoDecodec::updateDisplay);
    QObject::connect(m_pThread, SIGNAL(started()), m_pTimer, SLOT(start()));
    QObject::connect(m_pThread, &QThread::destroyed, m_pTimer, &QTimer::deleteLater);
}

void GPUFilterVideoDecodec::startTimer(int interval)
{
    m_pTimer->setInterval(interval);

    if (!m_pThread->isRunning())
        m_pThread->start();
}

void GPUFilterVideoDecodec::stopTimer(void)
{
    m_pThread->quit();
    m_pThread->wait();
}


// open video file
bool GPUFilterVideoDecodec::openVideoFile(const QString& fileName)
{
    // open input format
    int result = avformat_open_input(&m_pFormatContext, fileName.toLocal8Bit().data(), nullptr, nullptr);
    if (result != 0 || m_pFormatContext == nullptr)
        return false;

    // fill stream info
    if (avformat_find_stream_info(m_pFormatContext, nullptr) < 0)
    {
        avformat_close_input(&m_pFormatContext);
        return false;
    }

    // get video and audio stream id
    int streamCount = m_pFormatContext->nb_streams;
    for (int i = 0; i < streamCount; ++i)
    {
        if (m_pFormatContext->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO)
        {
            m_nVideoIndex = i;
            break;
        }
    }

    if (m_nVideoIndex < 0)
    {
        avformat_close_input(&m_pFormatContext);
        return false;
    }

    // open video codec
    if (!openVideoCodec())
    {
        avformat_close_input(&m_pFormatContext);
        return false;
    }

    AVStream* pStram = m_pFormatContext->streams[m_nVideoIndex];
    qDebug() << pStram->avg_frame_rate.den << pStram->avg_frame_rate.num;

    m_fps = pStram->avg_frame_rate.num * 1.0 / pStram->avg_frame_rate.den;

    // Start Timer
    startTimer(1000.0 / m_fps);

    return true;
}

// get Frame Data
bool GPUFilterVideoDecodec::getYUVData(QVector<QByteArray>& yuvData, int& type)
{
    if (m_semaphore.available() >= m_nTotal)
        return false;

    yuvData.clear();

    AVFrame* pFrame = m_frames[m_nStartIndex];
//    qDebug() << "released:" << m_nStartIndex << (pFrame == nullptr);
    m_nStartIndex++;
    if (m_nStartIndex >= m_nTotal)
        m_nStartIndex = 0;

    if (pFrame == nullptr)
    {
        m_semaphore.release();
        return false;
    }

    if (pFrame->format == AVPixelFormat::AV_PIX_FMT_YUV420P || \
        pFrame->format == AVPixelFormat::AV_PIX_FMT_YUVJ420P)
    {
        QByteArray yData, uData, vData;
        yData.append((const char*)pFrame->data[0], pFrame->width * pFrame->height);
        uData.append((const char*)pFrame->data[1], pFrame->width / 2 * pFrame->height / 2);
        vData.append((const char*)pFrame->data[2], pFrame->width / 2 * pFrame->height / 2);

        yuvData << yData << uData << vData;
        type = 0;
    }

    m_semaphore.release();

    av_frame_free(&pFrame);

    return true;
}

void GPUFilterVideoDecodec::getVideoSize(int& width, int& height)
{
    width = m_nVideoWidth;
    height = m_nVideoHeight;
}

// open video decodec
bool GPUFilterVideoDecodec::openVideoCodec(void)
{
    // find video codec
    m_pVideoCodecContext = m_pFormatContext->streams[m_nVideoIndex]->codec;
    AVCodec* videoCodec = avcodec_find_decoder(m_pVideoCodecContext->codec_id);
    if (videoCodec == nullptr)
    {
        avformat_close_input(&m_pFormatContext);
        return false;
    }

    // open video codec
    int result = avcodec_open2(m_pVideoCodecContext, videoCodec, nullptr);
    if (result != 0)
    {
        avformat_close_input(&m_pFormatContext);
        return false;
    }

    // fill info
    m_nVideoWidth = m_pVideoCodecContext->width;
    m_nVideoHeight = m_pVideoCodecContext->height;
    m_pixelFormat = m_pVideoCodecContext->pix_fmt;

    return true;
}

bool GPUFilterVideoDecodec::decodecVideo(AVPacket* packet)
{
    AVFrame *tempFrame = av_frame_alloc();

    // read packet
    if (avcodec_send_packet(m_pVideoCodecContext, packet))
    {
        av_frame_free(&tempFrame);
        return false;
    }

    // recv frame
    if (avcodec_receive_frame(m_pVideoCodecContext, tempFrame))
    {
        av_frame_free(&tempFrame);
        return false;
    }

    m_semaphore.acquire(1);
    m_frames[m_nEndIndex] = tempFrame;
//    qDebug() << "acquired:" << m_nEndIndex;
    m_nEndIndex++;
    if (m_nEndIndex >= m_nTotal)
        m_nEndIndex = 0;

    return true;
}

void GPUFilterVideoDecodec::run(void)
{
    while (!this->isInterruptionRequested())
    {
        AVPacket pkt;
        int result = av_read_frame(m_pFormatContext, &pkt);
        if (result)
            break;

        // decode video and audio
        if (pkt.stream_index == m_nVideoIndex)
            decodecVideo(&pkt);

        av_packet_unref(&pkt);
    }
}
