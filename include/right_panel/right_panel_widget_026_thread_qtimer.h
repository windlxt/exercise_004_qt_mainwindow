#pragma once

#include <iterator> // IWYU pragma: keep
#include <qabstractspinbox.h>
#include <qpushbutton.h>
#include <qstackedwidget.h>
#include <qstyle.h>

class QTextEdit;
class QLabel;
class QLineEdit;
class QScrollArea;

#include <QObject>
#include <QTimer>
#include <QDebug>
#include <QThread>
#include <mutex>
#include <condition_variable>
#include <deque>
#include <atomic>

class WorkerQTimer : public QObject
{
    Q_OBJECT
public:
    explicit WorkerQTimer(QObject *parent = nullptr) : QObject(parent) {}

signals:
    void finished();
    void logMsg(QString msg); // 发给UI日志

public slots:
    // 主线下发的控制命令：暂停/恢复
    void setPause(bool pause)
    {
        m_pause = pause;
        qDebug() << "【子线程槽 setPause】threadId:" << QThread::currentThreadId() << " pause=" << pause;
    }

    // 启动定时器，开始周期性生产
    void startWork()
    {
        qDebug() << "【startWork】threadId:" << QThread::currentThreadId();
        m_stop = false;
        m_pause = false;

        m_timer = new QTimer(this);
        // 定时器timeout信号，触发单次生产
        connect(m_timer, &QTimer::timeout, this, &WorkerQTimer::produceOne);
        m_timer->start(700); // 700ms执行一次produceOne
    }

    // 停止任务
    void stopWork()
    {
        m_stop = true;
        if(m_timer)
        {
            m_timer->stop();
            m_timer->deleteLater();
            m_timer = nullptr;
        }
        emit finished();
    }

private slots:
    void produceOne()
    {
        if(m_stop.load()) return;
        if(m_pause.load()) return;

        std::unique_lock<std::mutex> lock(m_mtx);
        // 仓库满了，不生产，等待空位
        m_cv.wait_for(lock, std::chrono::milliseconds(0), [this](){
            return m_deque.size() < m_buf_cap;
        });
        if(m_deque.size() >= m_buf_cap)
        {
            qDebug() << "仓库满，跳过本次生产";
            return;
        }

        int val = rand() % 1000;
        m_deque.push_back(val);
        lock.unlock();
        m_cv.notify_all();

        qDebug() << "生产数据：" << val << " threadId:" << QThread::currentThreadId();
        emit logMsg(QString("生产：%1").arg(val));
    }

private:
    QTimer* m_timer{};
    std::atomic<bool> m_stop{false};
    std::atomic<bool> m_pause{false};

    std::mutex m_mtx;
    std::condition_variable m_cv;
    std::deque<int> m_deque;
    const int m_buf_cap = 10;
};

class RightStack026: public QWidget
{
    Q_OBJECT
public:
    RightStack026(QWidget* parent=nullptr);
    ~RightStack026() override;
    
private:
    void setupUI();
    void connectComponents();
    
signals:
    void sigSetPause(bool pause);

private slots:
    void onStart();
    void onPause();
    void onResume();
    void appendLog(QString msg);

private:
    QThread* m_thread{};
    WorkerQTimer* m_worker{};
    QPushButton* m_btnStart{};
    QPushButton* m_btnPause{};
    QPushButton* m_btnResume{};
    QTextEdit* m_txtLog{};     
};