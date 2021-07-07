#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QPlainTextEdit>
#include <QPushButton>
#include <QSpinBox>
#include <QLabel>
#include <QComboBox>
#include <QCheckBox>

class PerformanceTestObject;
class Widget : public QWidget
{
    Q_OBJECT

public:
    enum SizeType
    {
        t_Mode1,        // 360x240
        t_Mode2,        // 640x360
        t_Mode3,        // 720x480
        t_Mode4,        // 1280x720
        t_Mode5,        // 1920x1080
        t_Mode6,        // 2048x1080
        t_Mode7         // 4096x3112
    };

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

    void appendText(const QString& str);

private:
    QPlainTextEdit* m_pTextEdit = nullptr;
    PerformanceTestObject* m_pPerformanceTestObject = nullptr;

    QWidget* createConfigWidget(void);

    QSpinBox* m_pWidthSpinBox = nullptr;
    QSpinBox* m_pHeightSpinBox = nullptr;
    QComboBox* m_pSizeComboBox = nullptr;
    QCheckBox* m_pCheckBox = nullptr;

    SizeType m_sizeType;
    QString sizeTypeToString(SizeType type);
    void updateCurrentSizeType(void);

private slots:
    void onClickedButton(void);
    void onComboBoxCurrentIndexChanged(int index);
    void onCheckBoxCheckStatusChanged(int status);
};

#endif // WIDGET_H
