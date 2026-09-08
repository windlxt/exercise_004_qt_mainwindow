#include <iterator> // IWYU pragma: keep
#include <qboxlayout.h>
#include <qpushbutton.h>
#include <qstackedwidget.h>
#include <left_panel_widget_002.h>

LeftStack002::LeftStack002(QWidget* parent)
    : QWidget(parent)
{
    setupUI();
    connectComponents();
}

void LeftStack002::setupUI()
{
    QVBoxLayout* layout = new QVBoxLayout();
    QPushButton* m_btn_example = new QPushButton("第二个例子！！");
    layout->addWidget(m_btn_example);
    setLayout(layout);    
}

void LeftStack002::connectComponents()
{
    
}