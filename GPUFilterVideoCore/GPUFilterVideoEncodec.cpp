#include "GPUFilterVideoEncodec.h"

GPUFilterVideoEncodec::GPUFilterVideoEncodec(QObject* parent)
    :QObject(parent)
{
    av_register_all();
    avcodec_register_all();
}

GPUFilterVideoEncodec::~GPUFilterVideoEncodec()
{

}

void GPUFilterVideoEncodec::setCreateVideoInfo(const QString& fileName, const VideoInfo& videoInfo)
{
    m_fileName = fileName;
    m_createInfo = videoInfo;
}

bool GPUFilterVideoEncodec::startVideoEncodec(void)
{
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

    int result = avcodec_open2(m_pVideoCodecContext, m_pVideoCodec, nullptr);
    if (result < 0)
        return false;

    return createFormat();
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
    pVideoStream->time_base = { 1, m_createInfo.fts };
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

}

void GPUFilterVideoEncodec::writeImage(const QImage& image)
{

}
