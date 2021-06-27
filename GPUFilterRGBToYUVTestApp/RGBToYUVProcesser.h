#ifndef RGBTOYUVPROCESSER_H
#define RGBTOYUVPROCESSER_H

#include <QObject>

class RGBToYUVProcesser : public QObject
{
    Q_OBJECT

public:
    RGBToYUVProcesser(QObject* parent = nullptr);
    ~RGBToYUVProcesser();
};

#endif
