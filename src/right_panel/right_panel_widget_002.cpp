#include <iterator> // IWYU pragma: keep
#include <qboxlayout.h>
#include <qpushbutton.h>
#include <qstackedwidget.h>
#include <right_panel_widget_002.h>

RightStack002::RightStack002(QWidget* parent)
    : QWidget(parent)
{
    setupUI();
    connectComponents();
}

void RightStack002::setupUI()
{
    QVBoxLayout* layout = new QVBoxLayout();
    QPushButton* m_btn_example = new QPushButton("第二个例子！！");
    layout->addWidget(m_btn_example);
    setLayout(layout);    
}

void RightStack002::connectComponents()
{
    
}