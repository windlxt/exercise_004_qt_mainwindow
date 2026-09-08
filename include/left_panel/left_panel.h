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

private:
    QStackedWidget* m_stack_widget{nullptr};

    LeftStack001* left_stack_001; 
    LeftStack002* left_stack_002; 
    LeftStack003* left_stack_003; 
};