#ifndef GPUFILTERTEXTURE_H
#define GPUFILTERTEXTURE_H

#include <QObject>
#include <QtOpenGL>
#include "GPUFilterCore_global.h"

class GPUFilterPBO2;
class GPUFILTERCORE_EXPORT GPUFilterTexture : public QObject
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

    // set/get texture format
    void setImageFormat(ImageFormat format);
    ImageFormat getImageFormat(void);

    // Active
    void activeTexture(int textureID);

    // PBO About
    void setPBOEnabled(bool isUsedPBO);
    bool isPBOEnabled(void);


private:
    GLuint m_nTextureID = 0;
    FilterType m_filterType;
    ImageFormat m_imageFormat;

    GLuint coverToGLType(ImageFormat format);

    bool m_hasCreated = false;

    QByteArray m_imageData;
    int m_nWidth = 0;
    int m_nHeight = 0;
    void setImageDataToTexture(void);
    void updateImageDataToTexture(void);

    // PBO About
    bool m_isUsedPBO = false;
    GPUFilterPBO2* m_pPBO = nullptr;
};

#endif
