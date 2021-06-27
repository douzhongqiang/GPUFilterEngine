#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>

class GPUFilterVideoDecodec;
class YUVToRGBProcesser;
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

    YUVToRGBProcesser* m_pYUVToRGBProcesser = nullptr;
    bool m_isYUVToRGBInited = false;
    bool m_isRunningConver = false;
    QPushButton* m_YUVToRGBButton = nullptr;

private slots:
    void onClickedBroseVideoButton(void);
    void onClickedConverYUVToRGBButton(void);

    void onClickedBroseImageButton(void);
    void onClickedConverRGBToYUVButton(void);

    void onUpdateDisplay(void);
};
#endif // WIDGET_H
