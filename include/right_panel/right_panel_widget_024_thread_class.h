#pragma once

#include <deque>
#include <iterator> // IWYU pragma: keep
#include <qabstractspinbox.h>
#include <qobject.h>
#include <qpushbutton.h>
#include <qstackedwidget.h>
#include <qstyle.h>
#include <mutex>

class QTextEdit;

// 工作类
class Worker : public QObject
{
    Q_OBJECT
public:
    explicit Worker(QObject *parent = nullptr);
    void setSharedResource(std::deque<int>* dq,
                        std::mutex* mtx,
                        std::condition_variable* cv,
                        std::atomic<bool>* stop_flag);

public slots:
    void do_producer();
    void do_consumer();

signals:
    void signal_produce(QString msg);   //往 UI 发送文本消息
    void signal_consumer(QString msg);  //往 UI 发送文本消息
    void signal_finished();             //工作循环退出后发送，通知 UI 线程清理

private:
    // 共享对象，是指针，对象在RightStack024分配
    std::deque<int>* m_p_deque{};
    std::mutex* m_p_mtx{};
    std::condition_variable* m_p_cv{};
    std::atomic<bool>* m_p_stop{};
    constexpr static int BUF_MAX_CAP = 5;   //仓库的最大容量
};

// 操作展示窗口
class RightStack024 : public QWidget
{
    Q_OBJECT
public:
    explicit RightStack024(QWidget *parent = nullptr);
    ~RightStack024() override;

private:
    void setupUI();
    void connectComponents();
    void on_btn_start();
    void on_btn_stop();

private:
    QPushButton* m_btn_start{};
    QPushButton* m_btn_stop{};
    QTextEdit* m_txt_display_area{};

    QThread* m_thread_producer{};
    QThread* m_thread_consumer{};
    Worker* m_work_producer{};
    Worker* m_work_consumer{};

    // 共享资源 是对象
    std::deque<int> m_deque;
    std::mutex m_mutex;
    std::condition_variable m_cv;
    std::atomic<bool> m_stop_flag{false};    

    QMetaObject::Connection m_conn_produce;     //保存信号连接句柄，析构时手动断开
    QMetaObject::Connection m_conn_consumer;    //保存信号连接句柄，析构时手动断开
};