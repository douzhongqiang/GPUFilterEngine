#include "GPUFilterTexture.h"
#include "GPUFilterTool.h"

GPUFilterTexture::GPUFilterTexture(QObject* parent)
    :QObject(parent)
    , m_filterType(t_linear)
{

}

GPUFilterTexture::~GPUFilterTexture()
{
    release();
}

bool GPUFilterTexture::create(void)
{
    g_GPUFunc->glGenBuffers(1, &m_nTextureID);

    bind();

    g_GPUFunc->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    g_GPUFunc->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

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

    // Fill Data
    m_imageData.clear();
    int len = image.byteCount();
    m_imageData.append((const char*)tempImage.constBits(), len);
    m_nWidth = image.width();
    m_nHeight = image.height();

    // Set Data To Texture
    setImageDataToTexture();
}

void GPUFilterTexture::setImageDataToTexture(void)
{
    if (m_imageData.size() <= 0 || !m_hasCreated)
        return;

    this->bind();

    GLint type = coverToGLType(m_imageFormat);
    g_GPUFunc->glTexImage2D(GL_TEXTURE_2D, 0, type, m_nWidth, m_nHeight, \
                            0, type, GL_UNSIGNED_BYTE, m_imageData.data());

    this->unbind();
    m_imageData.clear();
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
    m_imageData.clear();
    m_nWidth = width;
    m_nHeight = height;
    int perCount = 0;
    if (m_imageFormat == t_RGB)
        perCount = 3;
    else if (m_imageFormat == t_RGBA)
        perCount = 4;
    else
        perCount = 1;
    int len = width * height * perCount;
    m_imageData.append(pData, len);

    setImageDataToTexture();
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
