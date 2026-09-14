#include <iterator> // IWYU pragma: keep
#include <qboxlayout.h>
#include <qdebug.h>
#include <qstackedwidget.h>
#include <qwidget.h>
#include <right_panel.h>
#include <right_panel_widget_001.h>
#include <right_panel_widget_002.h>
#include <right_panel_widget_003.h>
#include <right_panel_widget_021_producer_consumer.h>
#include <right_panel_widget_022_system_font.h>
#include <right_panel_widget_023_time_decorator.h>
#include <right_panel_widget_024_thread_worker.h>
#include <right_panel_widget_025_concurrent_futurewatcher.h>
#include <right_panel_widget_026_thread_qtimer.h>
#include <right_panel_widget_027_display_color.h>
#include "utility/msg_broker.h"

RightPanel::RightPanel(QWidget* parent)
    : QWidget(parent)
{
    setupUI();
    connectComponents();

    // 打开右侧面板的第1个页面
    BrokerMessage msg;
    msg.src = "m_btn_open_right_001";
    msg.cmd = "open_right_001";
    msg.payload = 100;
    emit MsgBroker::instance().onMessage(msg);
}

void RightPanel::setupUI()
{
    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->setContentsMargins(0,0,0,0);
    layout->setSpacing(0);
    m_stack_widget = new QStackedWidget(this);
    layout->addWidget(m_stack_widget);   

    ///////////////////////////////////////////////////////////////////
    // 1. 字符串与实例对象之间的映射
    cmdMap = {
        {"open_right_001", right_stack_001},
        {"open_right_002", right_stack_002},
        {"open_right_003", right_stack_003},
        {"open_right_021_producer_consumer", right_stack_021_producer_consumer},
        {"open_right_022_system_font", right_stack_022_system_font},
        {"open_right_023_time_decorator",right_stack_023_time_decorator},
        {"open_right_024_thread_worker_mutex",right_stack_024_thread_worker_mutex},
        {"open_right_025_concurrent_futurewatcher",right_stack_025_concurrent_futurewatcher},
        {"open_right_026_thread_qtimer",right_panel_widget_026_thread_qtimer},
        {"open_right_027_display_color",right_panel_widget_027_display_color}
    };
    
    //2. 注册函数工厂 ：cmd字符串 -> 创建对应子类////////////////////////////////    
    pageFactory["open_right_001"] = [](QWidget* parent)->QWidget*{
        return new RightStack001(parent);
    };
    pageFactory["open_right_002"] = [](QWidget* parent)->QWidget*{
        return new RightStack002(parent);
    };
    pageFactory["open_right_003"] = [](QWidget* parent)->QWidget*{
        return new RightStack003(parent);
    };
    pageFactory["open_right_021_producer_consumer"] = [](QWidget* parent)->QWidget*{
        return new RightStack021(parent);
    };
    pageFactory["open_right_022_system_font"] = [](QWidget* parent)->QWidget*{
        return new RightStack022(parent);
    };
    pageFactory["open_right_023_time_decorator"] = [](QWidget* parent)->QWidget*{
        return new RightStack023(parent);
    };
    pageFactory["open_right_024_thread_worker_mutex"] = [](QWidget* parent)->QWidget*{
        return new RightStack024(parent);
    };
    pageFactory["open_right_025_concurrent_futurewatcher"] = [](QWidget* parent)->QWidget*{
        return new RightStack025(parent);
    };
    pageFactory["open_right_026_thread_qtimer"] = [](QWidget* parent)->QWidget*{
        return new RightStack026(parent);
    };
    pageFactory["open_right_027_display_color"] = [](QWidget* parent)->QWidget*{
        return new RightStack027(parent);
    };

}

void RightPanel::connectComponents(){
    // 订阅全局消息
    connect(&MsgBroker::instance(), &MsgBroker::onMessage,
            this, [this](const BrokerMessage& msg){
                
        qDebug() << "RightPanel receive:" << msg.src << msg.cmd;
        
        auto it = cmdMap.find(msg.cmd); // it是迭代器
        if (it != cmdMap.end())
        {
            // 懒加载页面
            if (!it->second) 
            {
                // 从工厂查找创建器
                auto factoryIt = pageFactory.find(msg.cmd);
                if(factoryIt == pageFactory.end())
                {
                    qDebug() << "未注册页面:" << msg.cmd;
                    return;
                }
                // 调用工厂函数new对应子类
                QWidget* w = factoryIt->second(nullptr);
                it->second = w;
                m_stack_widget->addWidget(w);
                qDebug()<<"懒加载页面创建成功:" << msg.cmd;
            }

            m_stack_widget->setCurrentWidget(it->second);            
        }        
    }, Qt::QueuedConnection);

    // 预加载绑定信号与槽函数
    connect(this, &RightPanel::preload_right_panel, this ,&RightPanel::preload_right_stack_widget, Qt::QueuedConnection);
}


// 预加载页面
void RightPanel::preload_right_stack_widget()
{        
    //static 局部变量，第一次进入函数初始化，之后每次调用复用同一个变量
    static auto it_factory = pageFactory.begin (); 
    
    if (it_factory != pageFactory.end())
    {
        // 检查当前页面是否是已经创建好的open_right_001
        if(it_factory->first == "open_right_001")
        {
            qDebug()<< it_factory->first << " 页面已经创建,跳过！" ;
            ++it_factory;
            emit preload_right_panel(); // 继续触发下一轮，处理下一个页面
            return; // 本次不再创建页面
        }

        // 用工厂制造1个页面
        QWidget* w = it_factory->second(nullptr);

        auto it_cmdMap = cmdMap.find(it_factory->first);
        it_cmdMap->second = w;
        m_stack_widget->addWidget(w);

        qDebug()<<"预加载页面创建成功:" << it_factory->first; 

        ++it_factory; // 【只有处理完当前页面，才移动迭代器】
        emit preload_right_panel();
    }        
}