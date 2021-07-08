#ifndef GPUFILTERFRAMECONVERTCORE_H
#define GPUFILTERFRAMECONVERTCORE_H

#include <QObject>
#include <QMatrix4x4>
#include <QtOpenGL>
#include <QOpenGLContext>
#include "OpenGLCore/GPUFilterScene.h"
#include "OpenGLCore/GPUFilterMaterial.h"
#include "OpenGLCore/GPUFilterRectMesh.h"
#include "OpenGLCore/GPUFilterMesh.h"
#include "OpenGLCore/GPUFilterTexture.h"
#include "OpenGLCore/GPUFilterFBO.h"
#include "OpenGLCore/GPUFilterPBO2.h"
#include "OpenGLCore/GPUFilterCamera.h"
#include "OpenGLCore/GPUFilterPostProcessScene.h"
#include "GPUFilterFrameConvert_global.h"
extern "C"{
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
#include <libavutil/imgutils.h>
#include <libswresample/swresample.h>
#include <libavutil/opt.h>
}

class GPUFILTERFRAMECONVERT_EXPORT GPUFilterFrameConvertCore : public QObject
{
    Q_OBJECT

public:
    GPUFilterFrameConvertCore(QObject* parent = nullptr);
    ~GPUFilterFrameConvertCore();

    void rgb2yuv(AVFrame* rgbFrame, AVFrame* yuvFrame, bool isScale, int width, int height);

private:
    void init(void);
    void render(void);
    void resize(int w, int h);

private:
    void initCreate(void);

    void setImage(const QImage& image);
    QImage packImage(void);
    void packImage2(char* pY, char* pU, char* pV);

    QOffscreenSurface* m_pSurface = nullptr;
    QOpenGLContext* m_pContext = nullptr;

    GPUFilterTexture* m_pTexture = nullptr;
    GPUFilterPostProcessScene* m_pPostScene = nullptr;
    GPUFilterPBO2* m_pPBO = nullptr;

    void fillToFrame(const QImage& image, AVFrame* frame);

    int m_nWidth = 0;
    int m_nHeight = 0;

    bool m_isFirstRender = true;
};

#endif
