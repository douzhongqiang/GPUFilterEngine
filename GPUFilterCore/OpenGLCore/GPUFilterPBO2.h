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

public:
    GPUFilterPBO2(QObject* parent = nullptr);
    ~GPUFilterPBO2();

    void setPBOType(PBOType type);

    void create(int width, int height, bool isRGB = true);
    void getImage(QImage& image);
    void setImage(uchar* pImageData);

    void resize(int w, int h);

    void bind(void);
    void unbind(void);
    void swapPBOBuffer(void);

private:
    PBOType m_pboType;
    GLuint m_nPBO[2];

    int m_nWidth = 0;
    int m_nHeight = 0;
    int m_nChannelCount = 3;

    int m_nCurrentIndex = 0;

    void createPack(int width, int height);
    void createUnpack(int width, int height);
    void resizePack(int width, int height);
    void resizeUnpack(int width, int height);
    //uchar* m_pTempBufferData = nullptr;
};

#endif
