#include "GPUFilterVideoEncodec.h"
#include <QDebug>
//#include <QImage>

GPUFilterVideoEncodec::GPUFilterVideoEncodec(QObject* parent)
    :QObject(parent)
{
    av_register_all();
    avcodec_register_all();
}

GPUFilterVideoEncodec::~GPUFilterVideoEncodec()
{
    releaseAll();
}

void GPUFilterVideoEncodec::setCreateVideoInfo(const QString& fileName, const VideoInfo& videoInfo)
{
    m_fileName = fileName;
    m_createInfo = videoInfo;
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

    m_pVideoCodecContext->bit_rate = 400000;
    m_pVideoCodecContext->width = m_createInfo.width;
    m_pVideoCodecContext->height = m_createInfo.height;
    m_pVideoCodecContext->time_base.num = 1;
    m_pVideoCodecContext->time_base.den = m_createInfo.fts;
    m_pVideoCodecContext->gop_size = 10;
    m_pVideoCodecContext->max_b_frames = 1;
    m_pVideoCodecContext->pix_fmt = AV_PIX_FMT_YUV420P;

    if (codecId == AV_CODEC_ID_H264)
        av_opt_set(m_pVideoCodecContext->priv_data, "preset", "slow", 0);

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
    pVideoStream->id = m_pFormatContext->nb_streams - 1;
    pVideoStream->time_base.num = 1;
    pVideoStream->time_base.den = m_createInfo.fts;
    result = avcodec_parameters_from_context(pVideoStream->codecpar, m_pVideoCodecContext);
    if (result < 0)
    {
        avformat_free_context(m_pFormatContext);
        return false;
    }
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
        m_pFrame->format = AV_PIX_FMT_YUV420P;
        av_frame_get_buffer(m_pFrame, 32);
    }

    av_frame_make_writable(m_pFrame);
    av_frame_make_writable(m_pTempFrame);

    avpicture_fill((AVPicture*)m_pTempFrame, (const uint8_t*)image.constBits(), AV_PIX_FMT_RGB24, \
        image.width(), image.height());

    if (!rgbConverToYUV())
        return;

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
    pkt->duration = m_pFrame->pts;
    pkt->dts = m_pFrame->pts;
    pkt->pts = m_pFrame->pts;
    
    /*pkt.pts = av_rescale_q_rnd(pkt.pts, m_pVideoCodecContext->time_base, m_pFormatContext->streams[0]->time_base, AV_ROUND_INF);
    pkt.dts = av_rescale_q_rnd(pkt.dts, m_pVideoCodecContext->time_base, m_pFormatContext->streams[0]->time_base, AV_ROUND_INF);
    pkt.duration = av_rescale_q_rnd(pkt.duration, m_pVideoCodecContext->time_base, m_pFormatContext->streams[0]->time_base, AV_ROUND_INF);*/

    result = av_interleaved_write_frame(m_pFormatContext, pkt);
    av_packet_free(&pkt);
}

bool GPUFilterVideoEncodec::rgbConverToYUV(void)
{
    if (m_pSwsContext == nullptr)
    {
        m_pSwsContext = sws_getContext(m_createInfo.width, m_createInfo.height, AV_PIX_FMT_RGB24, \
            m_createInfo.width, m_createInfo.height, AV_PIX_FMT_YUV420P, SWS_BILINEAR, nullptr, nullptr, nullptr);
    }

    int result = sws_scale(m_pSwsContext, m_pTempFrame->data, m_pTempFrame->linesize, 0, m_createInfo.height, \
        m_pFrame->data, m_pFrame->linesize);
    return result > 0 ? true : false;
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
