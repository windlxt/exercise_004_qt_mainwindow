#include <iterator> // IWYU pragma: keep
#include <qboxlayout.h>
#include <qstackedwidget.h>
#include <right_panel.h>
#include <right_panel_widget_001.h>
#include <right_panel_widget_002.h>
#include <right_panel_widget_003.h>
#include <right_panel_widget_021_producer_consumer.h>
#include <right_panel_widget_022_system_font.h>
#include <right_panel_widget_023_time_decorator.h>
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
    
    // 1.在堆上生成独立窗口对象
    right_stack_001 = new RightStack001();
    right_stack_002 = new RightStack002();
    right_stack_003 = new RightStack003();
    right_stack_021_producer_consumer = new RightStack021();
    right_stack_022_system_font = new RightStack022();
    right_stack_023_time_decorator = new RightStack023();

    // 2.加入层叠控件
    m_stack_widget->addWidget(right_stack_001);
    m_stack_widget->addWidget(right_stack_002);
    m_stack_widget->addWidget(right_stack_003); 
    m_stack_widget->addWidget(right_stack_021_producer_consumer); 
    m_stack_widget->addWidget(right_stack_022_system_font);
    m_stack_widget->addWidget(right_stack_023_time_decorator);

    // 3.字符串与窗口对象之间的映射
    cmdMap = {
        {"open_right_001", right_stack_001},
        {"open_right_002", right_stack_002},
        {"open_right_003", right_stack_003},
        {"open_right_021_producer_consumer", right_stack_021_producer_consumer},
        {"open_right_022_system_font", right_stack_022_system_font},
        {"open_right_023_time_decorator",right_stack_023_time_decorator}
    };
}

void RightPanel::connectComponents(){
    // 订阅全局消息
    connect(&MsgBroker::instance(), &MsgBroker::onMessage,
            this, [this](const BrokerMessage& msg){
                
        // qDebug() << "RightPanel receive:" << msg.src << msg.cmd;
        
        auto it = cmdMap.find(msg.cmd);
        if (it != cmdMap.end())
        {
            m_stack_widget->setCurrentWidget(it->second);
        }        
    }, Qt::QueuedConnection);
}