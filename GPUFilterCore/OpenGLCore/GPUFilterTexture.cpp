#include "GPUFilterTexture.h"
#include "GPUFilterTool.h"
#include "GPUFilterPBO2.h"

GPUFilterTexture::GPUFilterTexture(QObject* parent)
    :QObject(parent)
    , m_filterType(t_linear)
{

}

GPUFilterTexture::~GPUFilterTexture()
{
//    release();
}

bool GPUFilterTexture::create(void)
{
    g_GPUFunc->glGenBuffers(1, &m_nTextureID);

    bind();

    g_GPUFunc->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    g_GPUFunc->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

    GLint filterType = GL_NEAREST;
    if (m_filterType == t_linear)
        filterType = GL_LINEAR;
    g_GPUFunc->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filterType);
    g_GPUFunc->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filterType);

    g_GPUFunc->glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    g_GPUFunc->glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
    g_GPUFunc->glPixelStorei(GL_UNPACK_SKIP_PIXELS, 0);
    g_GPUFunc->glPixelStorei(GL_UNPACK_SKIP_ROWS, 0);

    unbind();
    m_hasCreated = true;

    // If has Data And Fill Data
    setImageDataToTexture();

    return true;
}

bool GPUFilterTexture::hasCreated(void)
{
    return m_hasCreated;
}

void GPUFilterTexture::release(void)
{
    g_GPUFunc->glDeleteTextures(1, &m_nTextureID);

    if (m_pPBO)
        m_pPBO->release();
}

void GPUFilterTexture::bind(void)
{
    g_GPUFunc->glBindTexture(GL_TEXTURE_2D, m_nTextureID);
}

void GPUFilterTexture::unbind(void)
{
    g_GPUFunc->glBindTexture(GL_TEXTURE_2D, 0);
}

GLuint GPUFilterTexture::getId(void)
{
    return m_nTextureID;
}

// Get Size
int GPUFilterTexture::getWidth(void)
{
    return m_nWidth;
}

int GPUFilterTexture::getHeight(void)
{
    return m_nHeight;
}

void GPUFilterTexture::setImage(const QImage& image)
{
    QTime time;
    time.start();

    QImage tempImage = image;
    if (tempImage.format() == QImage::Format_RGB888)
        m_imageFormat = t_RGB;
    else if (tempImage.format() == QImage::Format_RGBA8888)
        m_imageFormat = t_RGBA;
    else
    {
        m_imageFormat = t_RGBA;
        tempImage = tempImage.convertToFormat(QImage::Format_RGBA8888);
    }
    int n1 = time.elapsed();

    // Fill Data
    int len = image.byteCount();
    int n2 = time.elapsed();

    bool isEnteredPBO = false;
    int n4 = 0;

    if (m_nWidth != image.width() && m_nHeight != image.height())
    {
        m_nWidth = image.width();
        m_nHeight = image.height();

        if (m_pImageData != nullptr)
        {
            delete[] m_pImageData;
        }
        m_pImageData = new char[len];
        memcpy(m_pImageData, tempImage.constBits(), len);
        n4 = time.elapsed();

        // Set Data To Texture
        setImageDataToTexture();
    }
    else
    {
        m_pImageData = (char*)image.constBits();
        n4 = time.elapsed();

        // Update For Image Data To Texture
        updateImageDataToTexture();

        isEnteredPBO = true;
    }

    int n3 = time.elapsed();

    qDebug() << __FUNCTION__ << n1 << n2 << n4 << n3 << isEnteredPBO;
}

void GPUFilterTexture::setImageDataToTexture(void)
{
    if (m_pImageData == nullptr || !m_hasCreated)
        return;

    this->bind();

    GLint type = coverToGLType(m_imageFormat);
    g_GPUFunc->glTexImage2D(GL_TEXTURE_2D, 0, type, m_nWidth, m_nHeight, \
                            0, type, GL_UNSIGNED_BYTE, m_pImageData);

    this->unbind();

    // Reset PBO
    delete m_pPBO;
    m_pPBO = nullptr;

    if (m_pImageData)
    {
        delete[] m_pImageData;
        m_pImageData = nullptr;
    }
}

void GPUFilterTexture::updateImageDataToTexture(void)
{
    if (m_pImageData == nullptr || !m_hasCreated)
        return;

    QTime time;
    time.start();

    this->bind();
    qDebug() << __FUNCTION__ << "bind" << time.elapsed();

    GLint type = coverToGLType(m_imageFormat);
    if (!m_isUsedPBO)
    {
        g_GPUFunc->glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, m_nWidth, m_nHeight, \
                                type, GL_UNSIGNED_BYTE, m_pImageData);
    }
    else
    {
        if (m_pPBO == nullptr)
        {
            m_pPBO = new GPUFilterPBO2(this);
            m_pPBO->setPBOSize(2);
            m_pPBO->setPBOType(GPUFilterPBO2::t_UnPack);

            if (m_imageFormat == t_BGR0)
                m_pPBO->create(m_nWidth, m_nHeight, GPUFilterPBO2::t_RGBA);
            else
                m_pPBO->create(m_nWidth, m_nHeight, GPUFilterPBO2::POBImageType(m_imageFormat));
        }

        m_pPBO->setImage((uchar*)m_pImageData);
    }

    this->unbind();
    qDebug() << __FUNCTION__ << "Unbind" << time.elapsed();
}

void GPUFilterTexture::setImage(const QString& image)
{
    QImage tempImage;
    if (!tempImage.load(image))
        return;

    this->setImage(tempImage);
}

void GPUFilterTexture::setImageData(const char* pData, int width, int height)
{
    int perCount = 0;
    if (m_imageFormat == t_RGB)
        perCount = 3;
    else if (m_imageFormat == t_RGBA || m_imageFormat == t_BGR0)
        perCount = 4;
    else
        perCount = 1;
    int len = width * height * perCount;

    if (m_nWidth != width && m_nHeight != height)
    {
        m_nWidth = width;
        m_nHeight = height;

        if (m_pImageData != nullptr)
        {
            delete[] m_pImageData;
            m_pImageData = nullptr;
        }
        m_pImageData = new char[len];
        memcpy(m_pImageData, pData, len);

        // Set Data To Texture
        setImageDataToTexture();

        if (m_pImageData)
        {
            delete[] m_pImageData;
            m_pImageData = nullptr;
        }
    }
    else
    {
        /*if (m_pImageData)
            memcpy(m_pImageData, pData, len);*/
        m_pImageData = (char*)pData;

        // Update For Image Data To Texture
        updateImageDataToTexture();
    }
}

void GPUFilterTexture::setFilterType(FilterType type)
{
    m_filterType = type;
}

GPUFilterTexture::FilterType GPUFilterTexture::getFilterType(void)
{
    return m_filterType;
}

void GPUFilterTexture::activeTexture(int textureID)
{
    g_GPUFunc->glActiveTexture(GL_TEXTURE0 + textureID);
    this->bind();
}

void GPUFilterTexture::setPBOEnabled(bool isUsedPBO)
{
    m_isUsedPBO = isUsedPBO;
}

bool GPUFilterTexture::isPBOEnabled(void)
{
    return m_isUsedPBO;
}

// set/get texture format
void GPUFilterTexture::setImageFormat(ImageFormat format)
{
    m_imageFormat = format;
}

GPUFilterTexture::ImageFormat GPUFilterTexture::getImageFormat(void)
{
    return m_imageFormat;
}

GLuint GPUFilterTexture::coverToGLType(ImageFormat format)
{
    if (format == t_RGB)
        return GL_RGB;
    else if (format == t_LUMINANCE)
        return GL_LUMINANCE;

    return GL_RGBA;
}
