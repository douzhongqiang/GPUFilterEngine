#include "GPUFilterVideoEncodec.h"
#include <QDebug>
//#include <QImage>

GPUFilterVideoEncodec::GPUFilterVideoEncodec(QObject* parent)
    :QThread(parent)
    , m_inputImageType(t_rgb)
{
    av_register_all();
    avcodec_register_all();

    initTimer();
    m_pGPUConvertCore = new GPUFilterFrameConvertCore(this);
}

GPUFilterVideoEncodec::~GPUFilterVideoEncodec()
{
    stopTimer();

    // Quit Thread
    if (this->isRunning())
    {
        this->requestInterruption();
        m_condition.wakeAll();
        this->wait();
    }

    releaseAll();
}

void GPUFilterVideoEncodec::setCreateVideoInfo(const QString& fileName, const VideoInfo& videoInfo)
{
    m_fileName = fileName;
    m_createInfo = videoInfo;
}

void GPUFilterVideoEncodec::setInputImageType(InputImageType type)
{
    m_inputImageType = type;
}

GPUFilterVideoEncodec::InputImageType GPUFilterVideoEncodec::getInputImageType(void)
{
    return m_inputImageType;
}

bool GPUFilterVideoEncodec::startVideoEncodec(void)
{
    m_nPtsIndex = 0;

    // Find Codec
    AVCodecID codecId = AV_CODEC_ID_H264;
    m_pVideoCodec = avcodec_find_encoder(codecId);
    if (m_pVideoCodec == nullptr)
        return false;

    // Create Encodec Context
    m_pVideoCodecContext = avcodec_alloc_context3(m_pVideoCodec);
    if (m_pVideoCodecContext == nullptr)
        return false;

    m_pVideoCodecContext->bit_rate = 4000000;
    m_pVideoCodecContext->width = m_createInfo.width;
    m_pVideoCodecContext->height = m_createInfo.height;
    m_pVideoCodecContext->time_base.num = 1;
    m_pVideoCodecContext->time_base.den = m_createInfo.fts;
    m_pVideoCodecContext->framerate.num = m_createInfo.fts;
    m_pVideoCodecContext->framerate.den = 1;
    m_pVideoCodecContext->gop_size = 12;
    //m_pVideoCodecContext->max_b_frames = 1;
    m_pVideoCodecContext->pix_fmt = (enum AVPixelFormat)AV_PIX_FMT_YUV420P;

    if (codecId == AV_CODEC_ID_H264)
    {
        av_opt_set(m_pVideoCodecContext->priv_data, "preset", "slow", 0);
        //av_opt_set(m_pVideoCodecContext->priv_data, "tune", "zerolatency", 0);
    }

    // Open
    int result = avcodec_open2(m_pVideoCodecContext, m_pVideoCodec, nullptr);
    if (result < 0)
        return false;

    // Write Head
    bool ret = createFormat();
    if (!ret)
        return false;

    result = avformat_write_header(m_pFormatContext, nullptr);
    if (result < 0)
        return false;

    this->start();
    // Start Timer
    startTimer(1000.0 / m_createInfo.fts);

    return true;
}

bool GPUFilterVideoEncodec::createFormat(void)
{
    QString formatFileName = m_fileName;
    // Create Format Context
    int result = avformat_alloc_output_context2(&m_pFormatContext, nullptr, nullptr, \
                                                formatFileName.toStdString().c_str());
    if (result < 0)
        return false;

    m_pOutputFormat = m_pFormatContext->oformat;

    // Add Stream
    AVStream* pVideoStream = avformat_new_stream(m_pFormatContext, m_pVideoCodec);
    if (pVideoStream == nullptr)
    {
        avformat_free_context(m_pFormatContext);
        return false;
    }

    result = avcodec_parameters_from_context(pVideoStream->codecpar, m_pVideoCodecContext);
    if (result < 0)
    {
        avformat_free_context(m_pFormatContext);
        return false;
    }
    pVideoStream->id = m_pFormatContext->nb_streams - 1;
    pVideoStream->time_base.num = 1;
    pVideoStream->time_base.den = m_createInfo.fts;
    pVideoStream->r_frame_rate.num = m_createInfo.fts;
    pVideoStream->r_frame_rate.den = m_createInfo.fts;
    av_dump_format(m_pFormatContext, 0, formatFileName.toStdString().c_str(), 1);

    if (m_pOutputFormat->flags & AVFMT_GLOBALHEADER)
        m_pVideoCodecContext->flags |= AV_CODEC_FLAG_GLOBAL_HEADER;

    // Open File
    if (!(m_pOutputFormat->flags & AVFMT_NOFILE))
    {
        result = avio_open(&m_pFormatContext->pb, formatFileName.toStdString().c_str(), AVIO_FLAG_WRITE);
        if (result < 0)
        {
            avformat_free_context(m_pFormatContext);
            return false;
        }
    }

    return true;
}

void GPUFilterVideoEncodec::endVideoEncodec(void)
{
    stopTimer();
    
    // Quit Thread
    if (this->isRunning())
    {
        this->requestInterruption();
        m_condition.wakeAll();
        this->wait();
    }

    AVPacket* pkt = av_packet_alloc();
    av_init_packet(pkt);

    int result = avcodec_receive_packet(m_pVideoCodecContext, pkt);
    while (result > 0)
    {
        av_packet_rescale_ts(pkt, m_pVideoCodecContext->time_base, m_pFormatContext->streams[0]->time_base);
        pkt->stream_index = m_pFormatContext->streams[0]->index;
        pkt->pos = -1;

        result = av_interleaved_write_frame(m_pFormatContext, pkt);
        result = avcodec_receive_packet(m_pVideoCodecContext, pkt);
    }
    av_packet_free(&pkt);

    av_write_trailer(m_pFormatContext);
    avio_closep(&m_pFormatContext->pb);
    releaseAll();
}

void GPUFilterVideoEncodec::writeImage(const QImage& image)
{
    // RGB Conver To YUV
    if (m_pFrame == nullptr)
    {
        m_pFrame = av_frame_alloc();
        m_pTempFrame = av_frame_alloc();

        m_pFrame->width = m_createInfo.width;
        m_pFrame->height = m_createInfo.height;
        m_pTempFrame->width = m_createInfo.width;
        m_pTempFrame->height = m_createInfo.height;
        m_pTempFrame->format = AV_PIX_FMT_RGB24;

        m_pFrame->format = AV_PIX_FMT_YUV420P;
        av_frame_get_buffer(m_pFrame, 32);
    }

    av_frame_make_writable(m_pFrame);
    av_frame_make_writable(m_pTempFrame);

    avpicture_fill((AVPicture*)m_pTempFrame, (const uint8_t*)image.constBits(), AV_PIX_FMT_RGB24, \
        image.width(), image.height());

    if (m_inputImageType == t_rgb)
    {
        bool result = rgbConverToYUV();
        if (!result)
            return;
    }
    else if (m_inputImageType == t_yuv420p)
    {
        int index = 0;
        // Copy Y Data
        for (int i = 0; i < m_pFrame->height; ++i)
        {
            int interval = i * m_pFrame->linesize[0];
            int srcInterval = i * m_pFrame->width;
            memset(m_pFrame->data[0] + interval, 0, m_pFrame->linesize[0]);
            memcpy(m_pFrame->data[0] + interval, image.constBits() + srcInterval, m_pFrame->width);
        }

        // Copy U Data
        index += m_pFrame->width * m_pFrame->height;
        for (int i = 0; i < m_pFrame->height / 2; ++i)
        {
            int interval = i * m_pFrame->linesize[1];
            int srcInterval = index + m_pFrame->width / 4 * 4 * i;

            memset(m_pFrame->data[1] + interval, 255 / 2, m_pFrame->linesize[1]);
            memcpy(m_pFrame->data[1] + interval, image.constBits() + srcInterval, m_pFrame->width / 2);
        }

        // Copy V Data
        //index += m_pFrame->width / 2 * m_pFrame->height / 2;
        for (int i = 0; i < m_pFrame->height / 2; ++i)
        {
            int interval = i * m_pFrame->linesize[2];
            int srcInterval = index + m_pFrame->width / 4 * 4 * i + m_pFrame->width / 4 * 4 / 2;

            memset(m_pFrame->data[2] + interval, 255 / 2, m_pFrame->linesize[2]);
            memcpy(m_pFrame->data[2] + interval, image.constBits() + srcInterval, m_pFrame->width / 2);
        }
    }

    AVPacket *pkt = av_packet_alloc();
    av_init_packet(pkt);

    // Create AVPacket
    m_pFrame->pts = m_nPtsIndex++;
    
    int result = avcodec_send_frame(m_pVideoCodecContext, m_pFrame);
    if (result < 0)
        return;

    result = avcodec_receive_packet(m_pVideoCodecContext, pkt);
    if (result < 0)
    {
        av_packet_free(&pkt);
        return;
    }

    // Write To Filte
    av_packet_rescale_ts(pkt, m_pVideoCodecContext->time_base, m_pFormatContext->streams[0]->time_base);
    pkt->stream_index = m_pFormatContext->streams[0]->index;
    pkt->pos = -1;

    result = av_interleaved_write_frame(m_pFormatContext, pkt);
    av_packet_free(&pkt);
}

void GPUFilterVideoEncodec::addImage(const QImage& image)
{
    {
        QMutexLocker locker(&m_mutex);
        m_imageList.push_back(image);
    }

    m_condition.wakeAll();
}

bool GPUFilterVideoEncodec::rgbConverToYUV(const QImage& image)
{
    // RGB Conver To YUV
    if (m_pFrame == nullptr)
    {
        m_createInfo.width = image.width();
        m_createInfo.height = image.height();

        m_pFrame = av_frame_alloc();
        m_pTempFrame = av_frame_alloc();

        m_pFrame->width = image.width();
        m_pFrame->height = image.height();
        m_pFrame->format = AV_PIX_FMT_YUV420P;
        av_frame_get_buffer(m_pFrame, 32);
    }

    av_frame_make_writable(m_pFrame);
    av_frame_make_writable(m_pTempFrame);

    avpicture_fill((AVPicture*)m_pTempFrame, (const uint8_t*)image.constBits(), AV_PIX_FMT_RGB24, \
        image.width(), image.height());

    return rgbConverToYUV();
}

bool GPUFilterVideoEncodec::rgbConverToYUV(void)
{
    if (m_isUsedGPU)
    {
        m_pGPUConvertCore->rgb2yuv(m_pTempFrame, m_pFrame, false, 0, 0);
        return true;
    }

    if (m_pSwsContext == nullptr)
    {
        m_pSwsContext = sws_getContext(m_createInfo.width, m_createInfo.height, AV_PIX_FMT_RGB24, \
            m_createInfo.width, m_createInfo.height, AV_PIX_FMT_YUV420P, SWS_BILINEAR, nullptr, nullptr, nullptr);
    }

    int result = sws_scale(m_pSwsContext, m_pTempFrame->data, m_pTempFrame->linesize, 0, m_createInfo.height, \
        m_pFrame->data, m_pFrame->linesize);

    // For Test 
    /*QByteArray byte;
    byte.append((const char*)m_pFrame->data[0], m_pFrame->width * m_pFrame->height);
    byte.append((const char*)m_pFrame->data[1], m_pFrame->width / 2 * m_pFrame->height / 2);
    byte.append((const char*)m_pFrame->data[2], m_pFrame->width / 2 * m_pFrame->height / 2);
    QImage image((const uchar *)byte.constData(), m_pFrame->width / 4, \
        m_pFrame->height + m_pFrame->height / 2, QImage::Format_RGBA8888);
    image.save("./bin/Test_png.png");
    image.save("./bin/Test_bmp.bmp");*/

    return result > 0 ? true : false;
}

void GPUFilterVideoEncodec::setUsedGPU(bool isUsedGPU)
{
    m_isUsedGPU = isUsedGPU;
}

bool GPUFilterVideoEncodec::isUsedGPU(void)
{
    return m_isUsedGPU;
}

void GPUFilterVideoEncodec::releaseAll(void)
{
    if (m_pFrame)
        av_frame_free(&m_pFrame);

    if (m_pTempFrame)
        av_frame_free(&m_pTempFrame);

    if (m_pSwsContext)
        sws_freeContext(m_pSwsContext);

    if (m_pVideoCodecContext)
        avcodec_free_context(&m_pVideoCodecContext);

    if (m_pFormatContext != nullptr)
        avformat_free_context(m_pFormatContext);

    m_pFrame = nullptr;
    m_pTempFrame = nullptr;
    m_pSwsContext = nullptr;
    m_pVideoCodecContext = nullptr;
    m_pFormatContext = nullptr;
}

void GPUFilterVideoEncodec::run(void)
{
    while (!this->isInterruptionRequested())
    {
        m_mutex.lock();
        while (m_imageList.isEmpty() && !this->isInterruptionRequested())
            m_condition.wait(&m_mutex);
        QList<QImage> tempImageList = m_imageList;
        m_imageList.clear();
        m_mutex.unlock();

        for (auto iter = tempImageList.begin(); iter != tempImageList.end(); ++iter)
        {
            QImage tempImage = *iter;
            writeImage(tempImage);
        }
    }
}

void GPUFilterVideoEncodec::initTimer(void)
{
    m_pTimer = new QTimer;
    m_pThread = new QThread(this);
    m_pTimer->setTimerType(Qt::PreciseTimer);
    m_pTimer->moveToThread(m_pThread);

    QObject::connect(m_pTimer, &QTimer::timeout, this, &GPUFilterVideoEncodec::requestInputImage);
    QObject::connect(m_pThread, SIGNAL(started()), m_pTimer, SLOT(start()));
    QObject::connect(m_pThread, &QThread::destroyed, m_pTimer, &QTimer::deleteLater);
}

void GPUFilterVideoEncodec::startTimer(int interval)
{
    m_pTimer->setInterval(interval);

    if (!m_pThread->isRunning())
        m_pThread->start();
}

void GPUFilterVideoEncodec::stopTimer(void)
{
    m_pThread->quit();
    m_pThread->wait();
}
