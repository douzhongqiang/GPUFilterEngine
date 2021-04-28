#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "GPUConverCore/GPURenderWidget.h"

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private:
    GPURenderWidget* m_pRenderWidget = nullptr;

private slots:
    void onClickedButton(void);
};

#endif // WIDGET_H
