#include <iterator> // IWYU pragma: keep
#include <qboxlayout.h>
#include <qpushbutton.h>
#include <qstackedwidget.h>
#include <left_panel_widget_001.h>
#include <utility/style.h>
#include "utility/msg_broker.h"

LeftStack001::LeftStack001(QWidget* parent)
    : QWidget(parent)
{
    setupUI();
    connectComponents();
}

void LeftStack001::setupUI()
{
    // setStyleSheet(StyleLeftPanel001);
    QVBoxLayout* layout = new QVBoxLayout();
    m_btn_open_right_001 = new QPushButton("控件学习");
    m_btn_open_right_002 = new QPushButton("算法");
    m_btn_open_right_003 = new QPushButton("右侧窗口【3】");  

    layout->addWidget(m_btn_open_right_001);
    layout->addWidget(m_btn_open_right_002);
    layout->addWidget(m_btn_open_right_003);
    layout->addStretch();    
    setLayout(layout);    
}

void LeftStack001::connectComponents()
{
    connect(m_btn_open_right_001, &QPushButton::clicked, this, [](){
        // 在左侧面板某个触发点（按钮点击等）调用
        BrokerMessage msg;
        msg.src = "m_btn_open_right_001";
        msg.cmd = "open_right_001";
        msg.payload = 100;
        emit MsgBroker::instance().onMessage(msg);
    });

    connect(m_btn_open_right_002, &QPushButton::clicked, this, [](){
        // 在左侧面板某个触发点（按钮点击等）调用
        BrokerMessage msg;
        msg.src = "m_btn_open_right_002";
        msg.cmd = "open_right_002";
        msg.payload = 100;
        emit MsgBroker::instance().onMessage(msg);
    });

    connect(m_btn_open_right_003, &QPushButton::clicked, this, [](){
        // 在左侧面板某个触发点（按钮点击等）调用
        BrokerMessage msg;
        msg.src = "m_btn_open_right_003";
        msg.cmd = "open_right_003";
        msg.payload = 100;
        emit MsgBroker::instance().onMessage(msg);
    });
}