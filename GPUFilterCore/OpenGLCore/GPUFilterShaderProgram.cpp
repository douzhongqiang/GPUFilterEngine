#include "GPUFilterShaderProgram.h"
#include <QDebug>

GPUFilterShaderProgram::GPUFilterShaderProgram(QObject* parent)
    :QObject(parent)
{

}

GPUFilterShaderProgram::~GPUFilterShaderProgram()
{

}

bool GPUFilterShaderProgram::create(const QString& vertexShader, const QString& fragmentShader)
{
    // Load Vertex Shader
    QString vertexShaderStr(vertexShader);
    QOpenGLShader* pVertexShader = new QOpenGLShader(QOpenGLShader::Vertex, this);
    bool result = pVertexShader->compileSourceFile(vertexShaderStr);
    if (!result)
    {
        pVertexShader->deleteLater();
        qInfo() << pVertexShader->log();
        return false;
    }

    // Load Fragment Shader
    QString fragmentShaderStr(fragmentShader);
    QOpenGLShader* pFragmentShader = new QOpenGLShader(QOpenGLShader::Fragment, this);
    result = pFragmentShader->compileSourceFile(fragmentShaderStr);
    if (!result)
    {
        pVertexShader->deleteLater();
        pFragmentShader->deleteLater();
        qInfo() << pFragmentShader->log();
        return false;
    }

    // Create Shader Program
    m_pShaderProgram = new QOpenGLShaderProgram(this);
    m_pShaderProgram->addShader(pVertexShader);
    m_pShaderProgram->addShader(pFragmentShader);
    result = m_pShaderProgram->link();
    if (!result)
    {
        pVertexShader->deleteLater();
        pFragmentShader->deleteLater();
        m_pShaderProgram->deleteLater();
        qInfo() << "Link Error!";
    }

    return result;
}

// bind and unbind
void GPUFilterShaderProgram::bind(void)
{
    if (m_pShaderProgram)
        m_pShaderProgram->bind();
}

void GPUFilterShaderProgram::unbind(void)
{
    if (m_pShaderProgram)
        m_pShaderProgram->release();
}

GLuint GPUFilterShaderProgram::getProgramID(void)
{
    if (m_pShaderProgram)
        return m_pShaderProgram->programId();

    return m_pShaderProgram->programId();
}

QOpenGLShaderProgram* GPUFilterShaderProgram::getShaderProgram(void)
{
    return m_pShaderProgram;
}
