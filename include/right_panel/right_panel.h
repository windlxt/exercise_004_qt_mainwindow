#pragma once

#include <iterator> // IWYU pragma: keep
#include <qstackedwidget.h>

class RightStack001;
class RightStack002;
class RightStack003;
class RightStack021;
class RightStack022;
class RightStack023;
class RightStack024;
class RightStack025;
class RightStack026;
class RightStack027;

class RightPanel: public QWidget
{
    Q_OBJECT
public:
    RightPanel(QWidget* parent=nullptr);

    QStackedWidget* stackWidget() const { return m_stack_widget; } 
    
    // 懒加载：打开指定面板
    void open_id_right_stack_widget(int i); 

signals:
    void preload_right_panel();    

public slots: 
    void preload_right_stack_widget();

private:
    void setupUI();
    void connectComponents();

    // 1.层叠面板
    QStackedWidget* m_stack_widget{nullptr};

    // 2.独立的显示窗口
    RightStack001* right_stack_001{nullptr};    
    RightStack002* right_stack_002{nullptr};
    RightStack003* right_stack_003{nullptr};
    RightStack021* right_stack_021_producer_consumer{nullptr};
    RightStack022* right_stack_022_system_font{nullptr};
    RightStack023* right_stack_023_time_decorator{nullptr};
    RightStack024* right_stack_024_thread_worker_mutex{nullptr};
    RightStack025* right_stack_025_concurrent_futurewatcher{nullptr};
    RightStack026* right_panel_widget_026_thread_qtimer{nullptr};
    RightStack027* right_panel_widget_027_display_color{nullptr};

    // 页面工厂：key=cmd名称，value=创建页面的函数
    using PageCreator = std::function<QWidget*(QWidget*)>;
    std::unordered_map<QString, PageCreator> pageFactory;
    // 字符串和窗口的映射
    std::unordered_map<QString, QWidget*> cmdMap;

    // 预加载：高频页面列表
    QList<std::function<void()>> m_right_taskList;
};