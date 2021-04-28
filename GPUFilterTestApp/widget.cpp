#include "widget.h"
#include <QVBoxLayout>
#include <QPushButton>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    m_pRenderWidget = new GPURenderWidget;
    m_pRenderWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    QVBoxLayout* pLayout = new QVBoxLayout(this);
    pLayout->addWidget(m_pRenderWidget);

    QPushButton* pButton = new QPushButton("Load");
    pLayout->addWidget(pButton);
    QObject::connect(pButton, &QPushButton::clicked, this, &Widget::onClickedButton);
}

Widget::~Widget()
{

}

void Widget::onClickedButton(void)
{
    m_pRenderWidget->setImage(QImage("./green-5919790_1280.jpg"));
}
