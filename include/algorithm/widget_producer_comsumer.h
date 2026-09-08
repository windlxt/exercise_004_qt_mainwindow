#pragma once

#include <iterator>     // IWYU pragma: keep
#include <QMainWindow>
#include <QPushButton>
#include <QTextEdit>
#include <QObject>
#include <QString>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <thread>
#include <atomic>
#include <optional>

// 生产者消费者核心类，纯标准C++，不依赖Qt
class ProducerConsumer
{
public:
    ProducerConsumer() = default;
    ~ProducerConsumer();

    // 启动生产者、消费者线程
    void start();
    // 请求停止，等待线程安全退出
    void stop();

    // 回调：产生一条消息，传给UI（会包装成Qt信号）
    using MessageCallback = std::function<void(const std::string&)>;
    void setMessageCallback(MessageCallback cb);

private:
    void producer_work();
    void consumer_work();

    std::queue<int> m_queue;
    std::mutex m_mtx;
    std::condition_variable m_cv;

    std::atomic<bool> m_running{false};
    std::optional<std::thread> m_producer;
    std::optional<std::thread> m_consumer;

    MessageCallback m_msg_cb;
};

class winProducerComsumer : public QMainWindow
{
    Q_OBJECT
public:
    winProducerComsumer(QWidget *parent = nullptr);
    ~winProducerComsumer() override;

signals:
    // 给UI追加文本的信号，跨线程安全
    void sigAppendText(const QString&);

private slots:
    void onStartClicked();
    void onStopClicked();
    void appendText(const QString& txt);

private:
    QPushButton* m_btn_start;
    QPushButton* m_btn_stop;
    QTextEdit* m_text_edit;

    ProducerConsumer m_pc;
};