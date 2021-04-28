#ifndef GPUFILTERDIRECTIONLIGHT_H
#define GPUFILTERDIRECTIONLIGHT_H

#include <QObject>
#include <QVector3D>
#include "GPUFilterLightBase.h"

class GPUFilterDirectionLight : public GPUFilterLightBase
{
    Q_OBJECT

public:
    GPUFilterDirectionLight(QObject* parent = nullptr);
    ~GPUFilterDirectionLight();

    void setDirection(const QVector3D& direction);
    QVector3D getDirection(void);

    void processShader(QOpenGLShaderProgram* pShaderProgram, int index) override;

    int getType(void) override;

private:
    QVector3D m_direction;
};

#endif
