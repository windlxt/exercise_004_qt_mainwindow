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
    QPushButton* m_btn_thread_producer_consumer{nullptr};
    QPushButton* m_btn_load_system_font{nullptr};
    QPushButton* m_btn_thread_time_decorator{nullptr};
    QPushButton* m_btn_thread_worker_mutex{nullptr};
    QPushButton* m_btn_thread_concurrent_futurewatcher{nullptr};
    QPushButton* m_btn_thread_qtimer{nullptr};
    QPushButton* m_btn_display_color{nullptr};
};