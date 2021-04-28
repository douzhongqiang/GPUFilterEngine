#ifndef GPUFILTERSHADERPROGRAM_H
#define GPUFILTERSHADERPROGRAM_H

#include <QObject>
#include <QOpenGLShader>
#include <QOpenGLShaderProgram>

class GPUFilterShaderProgram : public QObject
{
    Q_OBJECT

public:
    GPUFilterShaderProgram(QObject* parent = nullptr);
    ~GPUFilterShaderProgram();

    // Create Shader Program
    bool create(const QString& vertexShader, const QString& fragmentShader);

    // bind and unbind
    void bind(void);
    void unbind(void);

    // Get Program ID
    GLuint getProgramID(void);

    QOpenGLShaderProgram* getShaderProgram(void);

private:
    QOpenGLShaderProgram* m_pShaderProgram = nullptr;
};

#endif
