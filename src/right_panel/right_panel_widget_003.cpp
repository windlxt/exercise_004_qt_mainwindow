#include <iterator> // IWYU pragma: keep
#include <qboxlayout.h>
#include <qpushbutton.h>
#include <qstackedwidget.h>
#include <right_panel_widget_003.h>

RightStack003::RightStack003(QWidget* parent)
    : QWidget(parent)
{
    setupUI();
    connectComponents();
}

void RightStack003::setupUI()
{
    QVBoxLayout* layout = new QVBoxLayout();
    QPushButton* m_btn_example = new QPushButton("第三个例子！！！");
    layout->addWidget(m_btn_example);
    setLayout(layout);    
}

void RightStack003::connectComponents()
{
    
}