#include "algorithm/widget_producer_comsumer.h"
#include <QVBoxLayout>
#include <chrono>
#include <qcolor.h>
#include <sstream>
#include <string>

ProducerConsumer::~ProducerConsumer()
{
    stop();
}

void ProducerConsumer::setMessageCallback(MessageCallback cb)
{
    m_msg_cb = std::move(cb);
}

void ProducerConsumer::start()
{
    if (m_running.load()) return;
    m_running = true;

    m_producer.emplace(&ProducerConsumer::producer_work, this);
    m_consumer.emplace(&ProducerConsumer::consumer_work, this);
}

void ProducerConsumer::stop()
{
    m_running = false;
    m_cv.notify_all(); // 唤醒等待的线程

    // 清空回调：子线程就算还想打印日志，m_msg_cb为空就不会执行emit
    m_msg_cb = nullptr;

    if(m_producer && m_producer->joinable())
        m_producer->join();
    if(m_consumer && m_consumer->joinable())
        m_consumer->join();

    m_producer.reset();
    m_consumer.reset();

    // ===== 在锁保护下清空队列 =====
    {
        std::lock_guard<std::mutex> lock(m_mtx);
        std::queue<int>().swap(m_queue); // swap快速清空queue
    }
}

void ProducerConsumer::producer_work()
{
    int item = 0;
    while(m_running.load())
    {
        // if(!m_msg_cb) break;

        std::this_thread::sleep_for(std::chrono::milliseconds(600));

        {
            std::lock_guard<std::mutex> lock(m_mtx);

            if (!m_running.load())
            {
                break;
            }

            m_queue.push(item);        
        }

        if(m_msg_cb)
        {
            std::stringstream ss;
            ss << "[生产者] 生产: " << item;
            m_msg_cb(ss.str());
        }
        m_cv.notify_one();
        item++;
    }
}

void ProducerConsumer::consumer_work()
{
    while(m_running.load())
    {
        std::unique_lock<std::mutex> lock(m_mtx);
        // 条件变量：队列空并且还在运行，则等待
        m_cv.wait(lock, [this](){
            return !m_queue.empty() || !m_running.load();
        });

        // 退出条件：停止标记并且队列空
        if(!m_running.load() && m_queue.empty())
            break;

        // if(!m_msg_cb) break;

        if(!m_queue.empty())
        {
            auto val = m_queue.front();
            m_queue.pop();
            lock.unlock(); // 尽早释放锁，不要持有锁做耗时操作

            std::this_thread::sleep_for(std::chrono::milliseconds(900));
            if(m_msg_cb)
            {
                // std::stringstream ss;
                // ss << "[消费者] 消费: " << val;
                // m_msg_cb(ss.str());
                
                // std::string s = "[消费者] 消费: " + std::to_string(val);
                std::string s = std::format("[消费者] 消费: {:03d}", val);
                m_msg_cb(s);
            }
        }
    }
}

winProducerComsumer::winProducerComsumer(QWidget *parent)
    : QMainWindow(parent)
{
    this->setWindowTitle("现代C++生产者消费者 QtUI");
    auto* central = new QWidget;
    auto* layout = new QVBoxLayout(central);
    this->setCentralWidget(central);

    m_btn_start = new QPushButton("开始");
    m_btn_stop = new QPushButton("停止");
    m_text_edit = new QTextEdit;
    m_text_edit->setReadOnly(true);

    layout->addWidget(m_btn_start);
    layout->addWidget(m_btn_stop);
    layout->addWidget(m_text_edit);

    connect(m_btn_start, &QPushButton::clicked, this, &winProducerComsumer::onStartClicked);
    connect(m_btn_stop, &QPushButton::clicked, this, &winProducerComsumer::onStopClicked);
    // 跨线程信号槽自动QueuedConnection
    connect(this, &winProducerComsumer::sigAppendText, this, &winProducerComsumer::appendText);

    // // C++生产者消费者回调，收到消息后发射Qt信号
    // m_pc.setMessageCallback([this](const std::string& msg){
    //     emit sigAppendText(QString::fromStdString(msg));
    // });
}

winProducerComsumer::~winProducerComsumer()
{
    m_pc.stop();
}

void winProducerComsumer::onStartClicked()
{
    // C++生产者消费者回调，收到消息后发射Qt信号
    m_pc.setMessageCallback([this](const std::string& msg){
        emit sigAppendText(QString::fromStdString(msg));
    });

    m_pc.start();
    appendText("==== 任务已启动 ====");
}

void winProducerComsumer::onStopClicked()
{
    m_pc.stop();
    appendText("==== 任务已安全停止 ====");
}

void winProducerComsumer::appendText(const QString &txt)
{
    // m_text_edit->append(txt);

    QTextCursor cursor = m_text_edit->textCursor();
    // cursor.movePosition(QTextCursor::End); //光标跳到文档末尾

    QTextCharFormat fmt;
    QColor txtColor;
    if(txt.contains("生产")){
        txtColor = QColor("#d5dd3f");
    }
    else {
        txtColor = QColor("#eb5a0c");
    }
    
    fmt.setForeground(txtColor); //文字颜色
    // fmt.setBackground(QColor("#3eb286")); //文字块背景，可选
    fmt.setFont(QFont("隶书",20));

    cursor.insertText(txt + "\n", fmt); //插入带颜色文本+换行     
}