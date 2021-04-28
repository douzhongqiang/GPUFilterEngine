#ifndef GPUFILTERTEXTURE_H
#define GPUFILTERTEXTURE_H

#include <QObject>
#include <QtOpenGL>

class GPUFilterTexture : public QObject
{
    Q_OBJECT

public:
    enum FilterType
    {
        t_nearest,
        t_linear
    };

    enum ImageFormat
    {
        t_RGB,
        t_RGBA,
        t_LUMINANCE
    };

    enum TextureType
    {
        t_diffuse,
        t_specular
    };

public:
    GPUFilterTexture(QObject* parent = nullptr);
    ~GPUFilterTexture();

    bool create(void);
    void release(void);
    bool hasCreated(void);

    // bind And unbind
    void bind(void);
    void unbind(void);
    GLuint getId(void);

    // Get Size
    int getWidth(void);
    int getHeight(void);

    // Set Data
    void setImage(const QImage& image);
    void setImage(const QString& image);
    void setImageData(const char* pData, int width, int height);

    // set/get FilterType
    void setFilterType(FilterType type);        // This Function Called, Before create
    FilterType getFilterType(void);

    // set/get Texture Type
    void setTextureType(TextureType type);
    TextureType getTextureType(void);

    // set/get texture format
    void setImageFormat(ImageFormat format);
    ImageFormat getImageFormat(void);

    // Active
    void activeTexture(int textureID);

private:
    GLuint m_nTextureID = 0;
    FilterType m_filterType;
    ImageFormat m_imageFormat;
    TextureType m_textureType;

    GLuint coverToGLType(ImageFormat format);

    bool m_hasCreated = false;

    QByteArray m_imageData;
    int m_nWidth = 0;
    int m_nHeight = 0;
    void setImageDataToTexture(void);
};

#endif
