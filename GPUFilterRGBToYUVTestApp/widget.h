#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>

class GPUFilterVideoDecodec;
class YUVToRGBProcesser;
class RGBToYUVProcesser;
class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private:
    QLineEdit* m_pVideoFilePathLineEdit = nullptr;
    QLineEdit* m_pImagePathLineEdit = nullptr;

    void init(void);
    GPUFilterVideoDecodec* m_pVideoDecodec = nullptr;

    // YUV To RGB About
    YUVToRGBProcesser* m_pYUVToRGBProcesser = nullptr;
    bool m_isYUVToRGBInited = false;
    bool m_isRunningConver = false;
    QPushButton* m_YUVToRGBButton = nullptr;

    // RGB To YUV About
    RGBToYUVProcesser* m_pRGBToYUVProcesser = nullptr;
    bool m_isRGBToYUVInited = false;
    QPushButton* m_RGBToYUVButton = nullptr;

private slots:
    void onClickedBroseVideoButton(void);
    void onClickedConverYUVToRGBButton(void);

    void onClickedBroseImageButton(void);
    void onClickedConverRGBToYUVButton(void);

    void onUpdateDisplay(void);
};
#endif // WIDGET_H
