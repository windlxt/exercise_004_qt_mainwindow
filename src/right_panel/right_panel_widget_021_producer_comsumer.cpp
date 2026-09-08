#include <iterator> // IWYU pragma: keep
#include <qboxlayout.h>
#include <qpushbutton.h>
#include <qstackedwidget.h>
#include <right_panel_widget_021_producer_comsumer.h>
#include <algorithm/widget_producer_comsumer.h>

RightStack021::RightStack021(QWidget* parent)
    : QWidget(parent)
{
    setupUI();
    connectComponents();
}

void RightStack021::setupUI()
{
    QVBoxLayout* layout = new QVBoxLayout();
    m_winProducerComsumer = new winProducerComsumer();
    layout->addWidget(m_winProducerComsumer);
    setLayout(layout);    
}

void RightStack021::connectComponents()
{
    
}