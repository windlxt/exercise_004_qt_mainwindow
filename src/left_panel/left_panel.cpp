#include <iterator> // IWYU pragma: keep
#include <qboxlayout.h>
#include <qlabel.h>
#include <qstackedwidget.h>
#include <left_panel.h>
#include <left_panel_widget_001.h>
#include <left_panel_widget_002.h>
#include <left_panel_widget_003.h>

LeftPanel::LeftPanel(QWidget* parent)
    : QWidget(parent)
{
    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->setContentsMargins(0,0,0,0);
    layout->setSpacing(0);

    m_stack_widget = new QStackedWidget(this);

    layout->addWidget(m_stack_widget);
    
    left_stack_001 = new LeftStack001();
    left_stack_002 = new LeftStack002();
    left_stack_003 = new LeftStack003();

    m_stack_widget->addWidget(left_stack_001);
    m_stack_widget->addWidget(left_stack_002);
    m_stack_widget->addWidget(left_stack_003);     
}