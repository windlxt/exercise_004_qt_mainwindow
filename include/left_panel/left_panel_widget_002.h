#pragma once

#include <iterator> // IWYU pragma: keep
#include <qpushbutton.h>
#include <qstackedwidget.h>

class LeftStack002: public QWidget
{
    Q_OBJECT
public:
    LeftStack002(QWidget* parent=nullptr);
    
private:
    void setupUI();
    void connectComponents();
    // 私有成员变量
    QPushButton* m_btn_producer_consumer{nullptr};
    QPushButton* m_btn_load_system_font{nullptr};
    QPushButton* m_btn_time_decorator{nullptr};
};