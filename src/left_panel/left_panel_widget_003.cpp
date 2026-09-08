#include <iterator> // IWYU pragma: keep
#include <qboxlayout.h>
#include <qpushbutton.h>
#include <qstackedwidget.h>
#include <left_panel_widget_003.h>

LeftStack003::LeftStack003(QWidget* parent)
    : QWidget(parent)
{
    setupUI();
    connectComponents();
}

void LeftStack003::setupUI()
{
    QVBoxLayout* layout = new QVBoxLayout();
    QPushButton* m_btn_example = new QPushButton("第三个例子！！！");
    layout->addWidget(m_btn_example);
    setLayout(layout);    
}

void LeftStack003::connectComponents()
{
    
}