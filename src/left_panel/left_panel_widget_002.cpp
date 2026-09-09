#include <iterator> // IWYU pragma: keep
#include <qboxlayout.h>
#include <qpushbutton.h>
#include <qstackedwidget.h>
#include <left_panel_widget_002.h>
#include "utility/msg_broker.h"

LeftStack002::LeftStack002(QWidget* parent)
    : QWidget(parent)
{
    setupUI();
    connectComponents();
}

void LeftStack002::setupUI()
{
    QVBoxLayout* layout = new QVBoxLayout();
    m_btn_producer_consumer = new QPushButton("生产消费多线程");
    m_btn_load_system_font = new QPushButton("显示系统字体");
    layout->addWidget(m_btn_producer_consumer);
    layout->addWidget(m_btn_load_system_font);
    layout->addStretch();
    setLayout(layout);    
}

void LeftStack002::connectComponents()
{
    connect(m_btn_producer_consumer, &QPushButton::clicked, this, [](){
        // 在左侧面板某个触发点（按钮点击等）调用
        BrokerMessage msg;
        msg.src = "m_btn_producer_consumer";
        msg.cmd = "open_right_021_producer_consumer";
        msg.payload = 100;
        emit MsgBroker::instance().onMessage(msg);
    });

    connect(m_btn_load_system_font, &QPushButton::clicked, this, [](){
        // 在左侧面板某个触发点（按钮点击等）调用
        BrokerMessage msg;
        msg.src = "m_btn_load_system_font";
        msg.cmd = "open_right_022_system_font";
        msg.payload = 100;
        emit MsgBroker::instance().onMessage(msg);
    });
}