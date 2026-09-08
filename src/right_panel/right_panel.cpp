#include <iterator> // IWYU pragma: keep
#include <qboxlayout.h>
#include <qstackedwidget.h>
#include <right_panel.h>
#include <right_panel_widget_001.h>
#include <right_panel_widget_002.h>
#include <right_panel_widget_003.h>
#include "utility/msg_broker.h"

RightPanel::RightPanel(QWidget* parent)
    : QWidget(parent)
{
    setupUI();
    connectComponents();
}

void RightPanel::setupUI()
{
    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->setContentsMargins(0,0,0,0);
    layout->setSpacing(0);
    m_stack_widget = new QStackedWidget(this);
    layout->addWidget(m_stack_widget);
    
    right_stack_001 = new RightStack001();
    right_stack_002 = new RightStack002();
    right_stack_003 = new RightStack003();

    m_stack_widget->addWidget(right_stack_001);
    m_stack_widget->addWidget(right_stack_002);
    m_stack_widget->addWidget(right_stack_003); 
}

void RightPanel::connectComponents(){
    // 订阅全局消息
    connect(&MsgBroker::instance(), &MsgBroker::onMessage,
            this, [this](const BrokerMessage& msg){
        // qDebug() << "RightPanel receive:" << msg.src << msg.cmd;
        if(msg.cmd == "open_right_001")
        {
            m_stack_widget->setCurrentWidget(right_stack_001);
        }
        else if(msg.cmd == "open_right_002")
        {
            m_stack_widget->setCurrentWidget(right_stack_002);
        }
        else if(msg.cmd == "open_right_003")
        {
            m_stack_widget->setCurrentWidget(right_stack_003);
        }
        
    }, Qt::QueuedConnection);
}