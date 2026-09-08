#pragma once

#include <iterator> // IWYU pragma: keep
#include <qstackedwidget.h>

class RightStack001;
class RightStack002;
class RightStack003;

class RightPanel: public QWidget
{
    Q_OBJECT
public:
    RightPanel(QWidget* parent=nullptr);

    QStackedWidget* stackWidget() const { return m_stack_widget; }  

private:
    void setupUI();
    void connectComponents();

    QStackedWidget* m_stack_widget{nullptr};
    RightStack001* right_stack_001;    
    RightStack002* right_stack_002;
    RightStack003* right_stack_003;
};