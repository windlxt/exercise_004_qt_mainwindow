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
    m_btn_producer_comsumer = new QPushButton("生产消费多线程");
    layout->addWidget(m_btn_producer_comsumer);
    setLayout(layout);    
}

void LeftStack002::connectComponents()
{
    connect(m_btn_producer_comsumer, &QPushButton::clicked, this, [](){
        // 在左侧面板某个触发点（按钮点击等）调用
        BrokerMessage msg;
        msg.src = "m_btn_producer_comsumer";
        msg.cmd = "open_right_021_producer_comsumer";
        msg.payload = 100;
        emit MsgBroker::instance().onMessage(msg);
    });
}