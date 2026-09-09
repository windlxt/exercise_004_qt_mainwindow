#pragma once

#include <iterator> // IWYU pragma: keep
#include <qstackedwidget.h>

class RightStack001;
class RightStack002;
class RightStack003;
class RightStack021;
class RightStack022;

class RightPanel: public QWidget
{
    Q_OBJECT
public:
    RightPanel(QWidget* parent=nullptr);

    QStackedWidget* stackWidget() const { return m_stack_widget; }  

private:
    void setupUI();
    void connectComponents();

    // 1.层叠面板
    QStackedWidget* m_stack_widget{nullptr};

    // 2.独立的显示窗口
    RightStack001* right_stack_001;    
    RightStack002* right_stack_002;
    RightStack003* right_stack_003;
    RightStack021* right_stack_021_producer_consumer;
    RightStack022* right_stack_022_system_font;

    // 字符串和窗口的映射
    std::unordered_map<QString, QWidget*> cmdMap;
};