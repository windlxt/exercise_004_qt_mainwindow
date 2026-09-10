#pragma once

#include <iterator> // IWYU pragma: keep
#include <qabstractspinbox.h>
#include <qpushbutton.h>
#include <qstackedwidget.h>
#include <qstyle.h>
#include <thread>

class QTextEdit;
class QLabel;
class QLineEdit;
class QScrollArea;

class RightStack023: public QWidget
{
    Q_OBJECT
public:
    RightStack023(QWidget* parent=nullptr);
    ~RightStack023() override;
    
private:
    void setupUI();
    void connectComponents();
    
    void work(int n);           // 用QMetaObject::invokeMethod传递1次信息
    std::thread m_work_thread_1;
    std::atomic<bool> m_stop_flag_1{false};

    void work_signal(int n);    // 用信号连接，多次传递信息
    std::thread m_work_thread_2;
    std::atomic<bool> m_stop_flag_2{false};

    // 私有成员变量
    QPushButton* m_btn_read_src;
    QLineEdit*   m_le_file_path;
    QTextEdit*   m_txt_code;    
    QLabel*      m_lb_time;    
    QLineEdit*   m_le_time_result;   
    QPushButton* m_btn_start_thread_1;
    QTextEdit*   m_txt_work_result;
    QPushButton* m_btn_start_thread_2;

    QLabel* m_lbl_profile;
    QScrollArea* m_scroll_profile;
signals:
    void msg_to_ui(QString msg);

private slots:
    // 点击按钮读取源码
    void on_read_src_clicked(); 
    // 测试 时间
    void on_start_thread_1();
    void on_start_thread_2();
     
};