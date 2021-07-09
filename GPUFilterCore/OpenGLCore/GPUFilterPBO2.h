#ifndef GPUFILTERPBO2_H
#define GPUFILTERPBO2_H

#include <QObject>
#include <QtOpenGL>
#include "GPUFilterCore_global.h"

class GPUFILTERCORE_EXPORT GPUFilterPBO2 : public QObject
{
    Q_OBJECT
public:
    enum PBOType
    {
        t_UnPack,
        t_Pack
    };

    enum POBImageType
    {
        t_RGB,
        t_RGBA,
        t_LUMINANCE
    };

public:
    GPUFilterPBO2(QObject* parent = nullptr);
    ~GPUFilterPBO2();

    void setPBOSize(int size) {
        m_nPBOSize = size;
    }
    void setSrcSize(int width, int height) {
        m_srcWidth = width;
        m_srcHeight = height;
    }
    void setYUVVisble(bool isVisible) {
        m_isYUV = isVisible;
    }

    void setPBOType(PBOType type);

    void create(int width, int height, bool isRGB = true);
    void create(int width, int height, POBImageType imageType);
    void getImage(QImage& image);
    void setImage(uchar* pImageData);

    void getImage(char* pY, char* pU, char* pV);

    void resize(int w, int h);

    void bind(void);
    void unbind(void);
    void swapPBOBuffer(void);
    void swapPBOBuffer2(void);

private:
    PBOType m_pboType;
    GLuint m_nPBO[8];
    int m_nPBOSize = 2;
    int m_index[2];

    int m_nWidth = 0;
    int m_nHeight = 0;
    int m_srcWidth = 0;
    int m_srcHeight = 0;

    int m_nChannelCount = 3;

    int m_nCurrentIndex = 0;
    int m_nCurrentIndex2 = 0;

    bool m_isYUV = false;

    void createPack(int width, int height);
    void createUnpack(int width, int height);
    void resizePack(int width, int height);
    void resizeUnpack(int width, int height);
    //uchar* m_pTempBufferData = nullptr;
};

#endif
