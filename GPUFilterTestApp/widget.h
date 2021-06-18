#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "GPUConverCore/GPURenderWidget.h"
#include "GPUFilterVideoDecodec.h"
#include "GPUFilterVideoPlayerWidget.h"

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private:
    GPUFilterVideoPlayerWidget* m_pRenderWidget = nullptr;
    GPUFilterVideoDecodec* m_pVideoDecodec = nullptr;

private slots:
    void onClickedButton(void);
    void onUpdateDisplay(void);

    void onClickedLoadModelButton(void);
    void onClickedLoadAnimationModelButton(void);
};

#endif // WIDGET_H
