#pragma once

#include <iterator> // IWYU pragma: keep
#include <qstackedwidget.h>

class LeftStack001;
class LeftStack002;
class LeftStack003;

class LeftPanel: public QWidget
{
    Q_OBJECT
public:
    LeftPanel(QWidget* parent=nullptr);

    QStackedWidget* stackWidget() const { return m_stack_widget; } 
    
    // 懒加载：打开ActivityBar指定的id面板，QHash<int, QPushButton*> m_id_to_btn
    void open_id_left_stack_widget(int i); 

signals:
    void preload_left_panel();  // 预加载信号
    
public slots: 
    void preload_left_stack_widget();   //预加载槽函数

private:
    void setupUI();
    void connectComponents();

    // 私有成员变量
    QStackedWidget* m_stack_widget{nullptr};

    LeftStack001* left_stack_001{nullptr}; 
    LeftStack002* left_stack_002{nullptr}; 
    LeftStack003* left_stack_003{nullptr};  
    
    // 高频页面列表
    QList<std::function<void()>> m_left_taskList;

};