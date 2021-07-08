#include "widget.h"
#include "PerformanceTestObject.h"
#include <QVBoxLayout>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , m_sizeType(t_Mode4)
{
    m_pPerformanceTestObject = new PerformanceTestObject(this);
    m_pPerformanceTestObject->setParentWidget(this);

    QVBoxLayout* pLayout = new QVBoxLayout(this);

    m_pTextEdit = new QPlainTextEdit;
    m_pTextEdit->setReadOnly(true);
    pLayout->addWidget(m_pTextEdit);

    QWidget* pConfigWidget = createConfigWidget();
    pLayout->addWidget(pConfigWidget);

    QPushButton* pPushButton = new QPushButton(tr("Start"));
    pLayout->addWidget(pPushButton);
    QObject::connect(pPushButton, &QPushButton::clicked, this, &Widget::onClickedButton);
}

Widget::~Widget()
{

}

QWidget* Widget::createConfigWidget(void)
{
    QWidget* pW = new QWidget;
    QVBoxLayout* pLayout = new QVBoxLayout(pW);
    pLayout->setSpacing(2);
    pLayout->setMargin(0);

    // Size
    QWidget* pSizeWidget = new QWidget;
    pLayout->addWidget(pSizeWidget);
    QHBoxLayout* pSizeLayout = new QHBoxLayout(pSizeWidget);
    pSizeLayout->setMargin(0);
    pSizeLayout->setSpacing(2);
    // Width
    QLabel* pWidthLabel = new QLabel(tr("Width:"));
    //pWidthLabel->setFixedWidth(80);
    pSizeLayout->addWidget(pWidthLabel);
    m_pWidthSpinBox = new QSpinBox;
    m_pWidthSpinBox->setMinimum(0);
    m_pWidthSpinBox->setMaximum(50000);
    m_pWidthSpinBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    pSizeLayout->addWidget(m_pWidthSpinBox);
    pSizeLayout->addSpacing(8);
    // Height
    QLabel* pHeightLabel = new QLabel(tr("Height:"));
    //pHeightLabel->setFixedWidth(80);
    pSizeLayout->addWidget(pHeightLabel);
    m_pHeightSpinBox = new QSpinBox;
    m_pHeightSpinBox->setMinimum(0);
    m_pHeightSpinBox->setMaximum(50000);
    m_pHeightSpinBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    pSizeLayout->addWidget(m_pHeightSpinBox);

    // ComboBox
    QWidget* pComboBoxWidget = new QWidget;
    pLayout->addWidget(pComboBoxWidget);
    QHBoxLayout* pComboBoxLayout = new QHBoxLayout(pComboBoxWidget);
    pComboBoxLayout->setMargin(0);
    pComboBoxLayout->setSpacing(2);
    QLabel* pComboBoxLabel = new QLabel(tr("Mode:"));
    //pWidthLabel->setFixedWidth(80);
    pComboBoxLayout->addWidget(pComboBoxLabel);
    m_pSizeComboBox = new QComboBox();
    m_pSizeComboBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    for (int i = (int)t_Mode1; i < (int)t_Mode7 + 1; ++i)
    {
        QString typeString = sizeTypeToString((SizeType)i);
        m_pSizeComboBox->addItem(typeString, i);
    }
    QObject::connect(m_pSizeComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onComboBoxCurrentIndexChanged(int)));
    updateCurrentSizeType();
    pComboBoxLayout->addWidget(m_pSizeComboBox);
    // Add Conver Mode
    m_pCheckBox = new QCheckBox(tr("Used GPU Conver"));
    pComboBoxLayout->addWidget(m_pCheckBox);
    m_pCheckBox->setChecked(true);
    QObject::connect(m_pCheckBox, &QCheckBox::stateChanged, this, &Widget::onCheckBoxCheckStatusChanged);

    // Resize Widget
    QWidget* pResizeWidget = new QWidget;
    pLayout->addWidget(pResizeWidget);
    QHBoxLayout* pResizeBoxLayout = new QHBoxLayout(pResizeWidget);
    pResizeBoxLayout->setMargin(0);
    pResizeBoxLayout->setSpacing(2);
    // Resize Checkbox
    m_pResizeCheckBox = new QCheckBox(tr("Resize Enabled"));
    QObject::connect(m_pResizeCheckBox, &QCheckBox::stateChanged, this, &Widget::onResizeableCheckBoxStatusChanged);
    pResizeBoxLayout->addWidget(m_pResizeCheckBox);
    m_pResizeCheckBox->setStyleSheet("background-color: red;");
    pResizeBoxLayout->addSpacing(8);
    // Resize To ComboBox
    QLabel* pResizeComboBoxLabel = new QLabel(tr("Resize To:"));
    //pWidthLabel->setFixedWidth(80);
    pResizeBoxLayout->addWidget(pResizeComboBoxLabel);
    m_pResizeComboBox = new QComboBox();
    m_pResizeComboBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    for (int i = (int)t_Mode1; i < (int)t_Mode7 + 1; ++i)
    {
        QString typeString = sizeTypeToString((SizeType)i);
        m_pResizeComboBox->addItem(typeString, i);
    }
    QObject::connect(m_pResizeComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onResizeComboBoxCurrentIndexChanged(int)));
    pResizeBoxLayout->addWidget(m_pResizeComboBox);

    return pW;
}

QString Widget::sizeTypeToString(SizeType type)
{
    if (type == t_Mode1)
        return "360x240";
    else if (type == t_Mode2)
        return "640x360";
    else if (type == t_Mode3)
        return "720x480";
    else if (type == t_Mode4)
        return "1280x720";
    else if (type == t_Mode5)
        return "1920x1080";
    else if (type == t_Mode6)
        return "2048x1080";
    else if (type == t_Mode7)
        return "4096x3112";
    
    return "";
}

void Widget::onClickedButton(void)
{
    m_pPerformanceTestObject->startTest();
}

void Widget::appendText(const QString& str)
{
    m_pTextEdit->appendPlainText(str);
}

void Widget::onComboBoxCurrentIndexChanged(int index)
{
    m_sizeType = (SizeType)(m_pSizeComboBox->currentData().toInt());
    updateCurrentSizeType();
}

void Widget::onResizeComboBoxCurrentIndexChanged(int index)
{
    SizeType type = (SizeType)(m_pResizeComboBox->currentData().toInt());

    QString sizeString = sizeTypeToString(type);
    QStringList sizeStringList = sizeString.split("x");
    int nWidth = sizeStringList.at(0).toInt();
    int nHeight = sizeStringList.at(1).toInt();
    m_pWidthSpinBox->setValue(nWidth);
    m_pHeightSpinBox->setValue(nHeight);

    m_pPerformanceTestObject->setResizeSize(nWidth, nHeight);
}

void Widget::onCheckBoxCheckStatusChanged(int status)
{
    bool isUsedGPU = true;
    if (status != Qt::Checked)
    {
        isUsedGPU = false;
    }

    m_pPerformanceTestObject->setUsedGPU(isUsedGPU);
}

void Widget::onResizeableCheckBoxStatusChanged(int status)
{
    bool isResizeable = false;
    if (status == Qt::Checked)
        isResizeable = true;

    m_pPerformanceTestObject->setResizeEnabled(isResizeable);
}

void Widget::updateCurrentSizeType(void)
{
    m_pSizeComboBox->blockSignals(true);

    int nSize = m_pSizeComboBox->count();
    for (int i = 0; i < nSize; ++i)
    {
        if (m_pSizeComboBox->itemData(i).toInt() == (int)m_sizeType)
        {
            m_pSizeComboBox->setCurrentIndex(i);
            break;
        }
    }
    m_pSizeComboBox->blockSignals(false);

    QString sizeString = sizeTypeToString(m_sizeType);
    QStringList sizeStringList = sizeString.split("x");
    int nWidth = sizeStringList.at(0).toInt();
    int nHeight = sizeStringList.at(1).toInt();
    m_pWidthSpinBox->setValue(nWidth);
    m_pHeightSpinBox->setValue(nHeight);

    m_pPerformanceTestObject->setSceneSize(nWidth, nHeight);
}
