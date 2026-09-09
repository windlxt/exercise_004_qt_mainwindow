#include <iterator> // IWYU pragma: keep
#include <qboxlayout.h>
#include <qpushbutton.h>
#include <qstackedwidget.h>
#include <algorithm/widget_producer_consumer.h>
#include <right_panel_widget_021_producer_consumer.h>

RightStack021::RightStack021(QWidget* parent)
    : QWidget(parent)
{
    setupUI();
    connectComponents();    
}

void RightStack021::setupUI()
{
    QVBoxLayout* layout = new QVBoxLayout();
    m_winProducerConsumer = new winProducerConsumer();
    layout->addWidget(m_winProducerConsumer);
    setLayout(layout);    
}

void RightStack021::connectComponents()
{
    
}