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

    m_btn_thread_producer_consumer = new QPushButton("【多线程1】  纯C++与Qt");
    m_btn_load_system_font = new QPushButton("显示系统字体");
    m_btn_thread_time_decorator = new QPushButton("【多线程2】  计时装饰器");
    m_btn_thread_class = new QPushButton("【多线程3】  常用类实现");
    m_btn_thread_concurrent_futurewatcher = new QPushButton("【多线程4】现代标准实现");

    layout->addWidget(m_btn_thread_producer_consumer);
    layout->addWidget(m_btn_load_system_font);
    layout->addWidget(m_btn_thread_time_decorator);
    layout->addWidget(m_btn_thread_class);
    layout->addWidget(m_btn_thread_concurrent_futurewatcher);
    layout->addStretch();
    setLayout(layout);    
}

void LeftStack002::connectComponents()
{
    // 生产消费多线程
    connect(m_btn_thread_producer_consumer, &QPushButton::clicked, this, [](){
        // 在左侧面板某个触发点（按钮点击等）调用
        BrokerMessage msg;
        msg.src = "m_btn_thread_producer_consumer";
        msg.cmd = "open_right_021_producer_consumer";
        msg.payload = 100;
        emit MsgBroker::instance().onMessage(msg);
    });

    // 显示系统字体
    connect(m_btn_load_system_font, &QPushButton::clicked, this, [](){
        // 在左侧面板某个触发点（按钮点击等）调用
        BrokerMessage msg;
        msg.src = "m_btn_load_system_font";
        msg.cmd = "open_right_022_system_font";
        msg.payload = 100;
        emit MsgBroker::instance().onMessage(msg);
    });

    // 计时装饰器
    connect(m_btn_thread_time_decorator, &QPushButton::clicked, this, [](){
        // 在左侧面板某个触发点（按钮点击等）调用
        BrokerMessage msg;
        msg.src = "m_btn_thread_time_decorator";
        msg.cmd = "open_right_023_time_decorator";
        msg.payload = 100;
        emit MsgBroker::instance().onMessage(msg);
    });

    // 多线程 用类的方式实现（move_to_thread）
    connect(m_btn_thread_class, &QPushButton::clicked, this, [](){
        // 在左侧面板某个触发点（按钮点击等）调用
        BrokerMessage msg;
        msg.src = "m_btn_thread_class";
        msg.cmd = "open_right_024_thread_class";
        msg.payload = 100;
        emit MsgBroker::instance().onMessage(msg);
    });

    // 多线程 现代标准实现（QtConcurrent + QFutureWatcher）
    connect(m_btn_thread_concurrent_futurewatcher, &QPushButton::clicked, this, [](){
        // 在左侧面板某个触发点（按钮点击等）调用
        BrokerMessage msg;
        msg.src = "m_btn_thread_concurrent_futurewatcher";
        msg.cmd = "open_right_025_concurrent_futurewatcher";
        msg.payload = 100;
        emit MsgBroker::instance().onMessage(msg);
    });
}