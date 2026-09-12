#include <iterator> // IWYU pragma: keep
#include <qboxlayout.h>
#include <qdebug.h>
#include <qlabel.h>
#include <qstackedwidget.h>
#include <left_panel.h>
#include <left_panel_widget_001.h>
#include <left_panel_widget_002.h>
#include <left_panel_widget_003.h>

LeftPanel::LeftPanel(QWidget* parent)
    : QWidget(parent)
{
    setupUI();
    connectComponents(); 
}

void LeftPanel::setupUI()
{
    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->setContentsMargins(0,0,0,0);
    layout->setSpacing(0);

    m_stack_widget = new QStackedWidget(this);    
    open_id_left_stack_widget(1);    // 装载第一个页面
    layout->addWidget(m_stack_widget);

    ////高频页面列表////////////////////////////////////
    // 等主窗口show，事件循环空闲后执行    
    m_left_taskList.append([&](){
        if(!left_stack_002) left_stack_002 = new LeftStack002();
        m_stack_widget->addWidget(left_stack_002);
    });
    m_left_taskList.append([&](){
        if(!left_stack_003) left_stack_003 = new LeftStack003();
        m_stack_widget->addWidget(left_stack_003);
    });
    ////高频页面列表///////////////////////////////////
}

void LeftPanel::connectComponents()
{
    // 预加载绑定信号与槽函数
    connect(this, &LeftPanel::preload_left_panel, this ,&LeftPanel::preload_left_stack_widget);
}

// 懒加载
void LeftPanel::open_id_left_stack_widget(int i)
{
    switch(i)
    {
        case 1:
            if (!left_stack_001) {
                // 第一次点击才创建页面
                left_stack_001 = new LeftStack001();
                m_stack_widget->addWidget(left_stack_001);
            }
            m_stack_widget->setCurrentWidget(left_stack_001);
            break;
        case 2:            
            if (!left_stack_002) {
                // 第一次点击才创建页面
                left_stack_002 = new LeftStack002();
                m_stack_widget->addWidget(left_stack_002);
            }
            m_stack_widget->setCurrentWidget(left_stack_002);
            break;
        case 3:
            if (!left_stack_003) {
                // 第一次点击才创建页面
                left_stack_003 = new LeftStack003();
                m_stack_widget->addWidget(left_stack_003);
            }
            m_stack_widget->setCurrentWidget(left_stack_003);
            break;
        default:
            qDebug()<<QString("没有建立与 %1 匹配的左侧面板，需完善！").arg(i);
    }
}

void LeftPanel::preload_left_stack_widget()
{    
    if(m_left_taskList.empty()) return;

    auto task = m_left_taskList.takeFirst();
    task();
    qDebug()<<"LeftPanel 预装载！";
    emit preload_left_panel();
    
}