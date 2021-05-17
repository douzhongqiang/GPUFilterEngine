#include "GPUFilterFBO.h"
#include "GPUFilterTool.h"
#include <QDebug>

GPUFilterFBO::GPUFilterFBO(QObject* parent)
    :QObject(parent)
{

}

GPUFilterFBO::~GPUFilterFBO()
{

}

bool GPUFilterFBO::create(void)
{
    g_GPUFunc->glGenFramebuffers(1, &m_frameBufferObjectId);
    this->bind();

    // Cereate Texture
    g_GPUFunc->glGenTextures(1, &m_textureId);
    g_GPUFunc->glBindTexture(GL_TEXTURE_2D, m_textureId);
    g_GPUFunc->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    g_GPUFunc->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    g_GPUFunc->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    g_GPUFunc->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    g_GPUFunc->glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_nWidth, m_nHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

    // Attach To FBO
    g_GPUFunc->glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_textureId, 0);

    // Create RBO(Render Buffer Object)
    g_GPUFunc->glGenRenderbuffers(1, &m_rboId);
    g_GPUFunc->glBindRenderbuffer(GL_RENDERBUFFER, m_rboId);
    g_GPUFunc->glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, m_nWidth, m_nHeight);
    // Attach To FBO
    g_GPUFunc->glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_rboId);

    m_isSuccessed = g_GPUFunc->glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE;
    qDebug() << m_isSuccessed;
    if (m_isSuccessed)
        unbind();

    return m_isSuccessed;
}

void GPUFilterFBO::bind(void)
{
    g_GPUFunc->glBindFramebuffer(GL_FRAMEBUFFER, m_frameBufferObjectId);
}

void GPUFilterFBO::unbind(void)
{
    g_GPUFunc->glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void GPUFilterFBO::release(void)
{
    if (!m_isSuccessed)
        return;

    g_GPUFunc->glDeleteFramebuffers(1, &m_frameBufferObjectId);
}

GLuint GPUFilterFBO::getTextureId(void)
{
    return m_textureId;
}

int GPUFilterFBO::width(void)
{
    return m_nWidth;
}

int GPUFilterFBO::height(void)
{
    return m_nHeight;
}
