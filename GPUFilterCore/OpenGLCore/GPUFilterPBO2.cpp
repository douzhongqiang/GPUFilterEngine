#include "GPUFilterPBO2.h"
#include "GPUFilterTool.h"

GPUFilterPBO2::GPUFilterPBO2(QObject* parent)
    :QObject(parent)
    , m_pboType(t_Pack)
{

}

GPUFilterPBO2::~GPUFilterPBO2()
{
    //delete[] m_pTempBufferData;
}

void GPUFilterPBO2::getImage(QImage& image)
{
    // Read Pixel Data To PBO
    g_GPUFunc->glBindBuffer(GL_PIXEL_PACK_BUFFER, m_nPBO[m_nCurrentIndex]);
    g_GPUFunc->glReadPixels(0, 0, m_nWidth, m_nHeight, GL_RGB, GL_UNSIGNED_BYTE, 0);

    swapPBOBuffer();

    // Read Data To RAM
    g_GPUFunc->glBindBuffer(GL_PIXEL_PACK_BUFFER, m_nPBO[m_nCurrentIndex]);
    GLubyte* src = (GLubyte*)g_GPUFunc->glMapBuffer(GL_PIXEL_PACK_BUFFER, GL_READ_ONLY);
    if (src)
    {
        image = QImage(src, m_nWidth, m_nHeight, QImage::Format_RGB888);
        g_GPUFunc->glUnmapBuffer(GL_PIXEL_PACK_BUFFER);
    }

    g_GPUFunc->glBindBuffer(GL_PIXEL_PACK_BUFFER, 0);
}

void GPUFilterPBO2::create(int width, int height)
{
    int bufferSize = width * height * m_nChannelCount;

    g_GPUFunc->glGenBuffers(2, m_nPBO);
    g_GPUFunc->glBindBuffer(GL_PIXEL_PACK_BUFFER, m_nPBO[0]);
    g_GPUFunc->glBufferData(GL_PIXEL_PACK_BUFFER, bufferSize, 0, GL_STREAM_READ);

    g_GPUFunc->glBindBuffer(GL_PIXEL_PACK_BUFFER, m_nPBO[1]);
    g_GPUFunc->glBufferData(GL_PIXEL_PACK_BUFFER, bufferSize, 0, GL_STREAM_READ);

    g_GPUFunc->glBindBuffer(GL_PIXEL_PACK_BUFFER, 0);

    /*if (m_pTempBufferData == nullptr)
        m_pTempBufferData = new uchar[bufferSize];*/
}

void GPUFilterPBO2::swapPBOBuffer(void)
{
    if (m_nCurrentIndex == 0)
        m_nCurrentIndex = 1;
    else
        m_nCurrentIndex = 0;
}

void GPUFilterPBO2::resize(int w, int h)
{
    if (w == m_nWidth && h == m_nHeight)
        return;

    m_nWidth = w;
    m_nHeight = h;
    int bufferSize = m_nWidth * m_nHeight * m_nChannelCount;

    g_GPUFunc->glBindBuffer(GL_PIXEL_PACK_BUFFER, m_nPBO[0]);
    g_GPUFunc->glBufferData(GL_PIXEL_PACK_BUFFER, bufferSize, 0, GL_STREAM_READ);

    g_GPUFunc->glBindBuffer(GL_PIXEL_PACK_BUFFER, m_nPBO[1]);
    g_GPUFunc->glBufferData(GL_PIXEL_PACK_BUFFER, bufferSize, 0, GL_STREAM_READ);

    g_GPUFunc->glBindBuffer(GL_PIXEL_PACK_BUFFER, 0);

    /*if (m_pTempBufferData != nullptr)
        delete[] m_pTempBufferData;
    m_pTempBufferData = new uchar[bufferSize];*/
}
