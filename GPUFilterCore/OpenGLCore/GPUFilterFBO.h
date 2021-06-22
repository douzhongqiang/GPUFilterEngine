#ifndef GPUFILTERFBO_H
#define GPUFILTERFBO_H

#include <QObject>
#include <QtOpenGL>
#include <QOpenGLFunctions_3_3_Core>
#include <QVector3D>
#include <QVector2D>
#include <QMatrix4x4>
#include "GPUFilterCore_global.h"

class GPUFILTERCORE_EXPORT GPUFilterFBO : public QObject
{
    Q_OBJECT

public:
    GPUFilterFBO(QObject* parent = nullptr);
    ~GPUFilterFBO();

    bool create(void);
    void bind(void);
    void unbind(void);
    void release(void);

    GLuint getTextureId(void);

    int width(void);
    int height(void);

    void setFBOSize(int width, int height);
    int getFBOWidth(void);
    int getFBOHeight(void);

private:
    GLuint m_frameBufferObjectId = 0;
    GLuint m_textureId = 0;
    GLuint m_rboId = 0;
    bool m_isSuccessed = false;

    int m_nWidth = 800;
    int m_nHeight = 600;
};

#endif
