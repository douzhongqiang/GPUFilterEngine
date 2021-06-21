#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QTimer>
#include "GPUConverCore/GPURenderWidget.h"
#include "GPUFilterVideoDecodec.h"
#include "GPUFilterVideoEncodec.h"
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

    // Record About
    QPushButton* m_pRecordVideoButton = nullptr;
    bool m_isRecording = false;
    GPUFilterVideoEncodec* m_pVideoEncodec = nullptr;

    int m_nWidth = 0;
    int m_nHeight = 0;

    QWidget* createVideoWidget(void);
    QWidget* createModelWidget(void);

private slots:
    void onClickedButton(void);
    void onClickedRecordButton(void);
    void onUpdateDisplay(void);

    void onClickedLoadModelButton(void);
    void onClickedLoadAnimationModelButton(void);

    void onRecordTimeout(void);
};

#endif // WIDGET_H
