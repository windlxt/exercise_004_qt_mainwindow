#include <iterator> // IWYU pragma: keep
#include <mutex>
#include <qboxlayout.h>
#include <qcontainerfwd.h>
#include <qframe.h>
#include <qlineedit.h>
#include <qnamespace.h>
#include <qobject.h>
#include <qpushbutton.h>
#include <qstackedwidget.h>
#include <qtextedit.h>
#include <right_panel_widget_024_thread_class.h>
#include <QLabel>
#include <QLineEdit>
#include <QFileDialog>
#include <QFile>
#include <QCoreApplication>
#include <utility/global_const_function.h>
#include <QScrollArea>
#include <utility/style.h>
#include <QThread>
#include <chrono>


// 工作类
Worker::Worker(QObject *parent)
    : QObject{parent}
{
    
}

void Worker::setSharedResource(std::deque<int> *dq,
                               std::mutex *mtx,
                               std::condition_variable *cv,
                               std::atomic<bool> *stop_flag)
{
    m_p_deque = dq;
    m_p_mtx = mtx;
    m_p_cv = cv;
    m_p_stop = stop_flag;
}

void Worker::do_producer()
{
    qDebug() << "生产者槽函数启动，线程ID:" << QThread::currentThreadId();
    int val;
    while (true)
    {
        if(m_p_stop->load())
        {
            qDebug() << "生产者收到停止请求，退出循环";
            break;
        }

        if(m_p_deque->size() == BUF_MAX_CAP)
        {
                qDebug()<<"仓库满了！！！";
                random_sleep(3000, 4000);
        }  

        val = rand() % 1000;
        std::unique_lock<std::mutex> lock(*m_p_mtx);        
        // 有界队列：队列满 并且 没有停止，就等待消费者取走数据        
        m_p_cv->wait(lock,[this](){               
            return m_p_deque->size() < BUF_MAX_CAP || m_p_stop->load();
        });
        // 唤醒后如果stop，直接退出，不生产
        if(m_p_stop->load())
        {
            break;
        }

        m_p_deque->push_back(val);
        lock.unlock();


        qDebug() << "生产数据:" << val;
        emit signal_produce(QString("生产数据: %1").arg(val));
        m_p_cv->notify_all();

        // 可中断等待
        std::unique_lock<std::mutex> lock_sleep(*m_p_mtx);
        m_p_cv->wait_for(lock_sleep, std::chrono::milliseconds(700), [this](){
            return m_p_stop->load();
        });
    }
    emit signal_finished();
}

void Worker::do_consumer()
{
    qDebug() << "消费者槽函数启动，线程ID:" << QThread::currentThreadId();
    while (true)
    {
        std::unique_lock<std::mutex> lock(*m_p_mtx);
        // 等待：队列有数据 OR 收到停止信号
        m_p_cv->wait(lock, [this](){
            return !m_p_deque->empty() || m_p_stop->load();
        });

        // 停止标记为true，不管队列有没有数据，直接退出
        if(m_p_stop->load())
        {
            qDebug() << "消费者收到停止请求，退出循环。队列中还剩余： "<<m_p_deque->size()<<" 个数据。";
            break;
        }

        int val = m_p_deque->front();
        m_p_deque->pop_front();
        lock.unlock();

        qDebug() << "消费数据:" << val;
        emit signal_consumer(QString("消费数据: %1").arg(val));
        m_p_cv->notify_all();   // 唤醒生产者，队列有空位了

        // 消费间隔900ms，可被stop提前唤醒
        std::unique_lock<std::mutex> lock_sleep(*m_p_mtx);
        m_p_cv->wait_for(lock_sleep, std::chrono::milliseconds(900), [this](){
            return m_p_stop->load();
        });
    }
    emit signal_finished();
}
/////////////////////////////////////////////////////////
// 操作展示窗口
RightStack024::RightStack024(QWidget* parent)
    : QWidget(parent)
{
    setupUI();
    connectComponents();
}

RightStack024::~RightStack024()
{
    m_stop_flag = true;
    m_cv.notify_all();

    if(m_thread_producer)
    {
        //while+cv 常驻循环的场景：**quit () 是多余的，甚至会造成误解，应该删掉 quit ()**
        //`quit()` 的作用：**给这个子线程的事件循环投递一个退出事件（QEvent::Quit）**。
        //m_thread_producer->quit();  
        m_thread_producer->wait();
    }
    if(m_thread_consumer)
    {
        //m_thread_consumer->quit();
        m_thread_consumer->wait();
    }

    // disconnect(m_conn_produce);
    // disconnect(m_conn_consumer);
}

void RightStack024::setupUI()
{
    setStyleSheet(R"(
        QPushButton:hover{
            background:#2c8d55;
        }
    )");
    QVBoxLayout* layout_root = new QVBoxLayout(this);
    QHBoxLayout* layout_btn = new QHBoxLayout();

    m_btn_start = new QPushButton("开始生产消费");
    m_btn_stop = new QPushButton("停止生产消费");
    m_txt_display_area = new QTextEdit();
    m_txt_display_area->setStyleSheet(StyleTextEdit);

    layout_btn->addWidget(m_btn_start);
    layout_btn->addWidget(m_btn_stop);
    layout_root->addLayout(layout_btn);
    layout_root->addWidget(m_txt_display_area);
}

void RightStack024::connectComponents()
{
    connect(m_btn_start, &QPushButton::clicked, this, &RightStack024::on_btn_start);
    connect(m_btn_stop, &QPushButton::clicked, this, &RightStack024::on_btn_stop);
}

void RightStack024::on_btn_start()
{
    std::srand(std::time(nullptr));

    if(m_thread_producer || m_thread_consumer)  // 防止重复启动
        return;

    m_stop_flag = false;
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_deque.clear();
    }

    m_thread_producer = new QThread();
    m_work_producer = new Worker();
    m_work_producer->moveToThread(m_thread_producer);
    m_work_producer->setSharedResource(&m_deque, &m_mutex, &m_cv, &m_stop_flag);

    m_thread_consumer = new QThread();
    m_work_consumer = new Worker();
    m_work_consumer->moveToThread(m_thread_consumer);
    m_work_consumer->setSharedResource(&m_deque, &m_mutex, &m_cv, &m_stop_flag);

    connect(m_thread_producer, &QThread::started, m_work_producer, &Worker::do_producer);
    connect(m_thread_consumer, &QThread::started, m_work_consumer, &Worker::do_consumer);

    connect(m_work_producer, &Worker::signal_finished, m_thread_producer, &QThread::quit);
    connect(m_work_consumer, &Worker::signal_finished, m_thread_consumer, &QThread::quit);

    //==== 用计数器，统计两个worker全部完成 ====
    std::atomic<int>* finish_counter = new std::atomic<int>(0);
    auto onWorkerFinished = [this, finish_counter]()
    {
        ++(*finish_counter);
        qDebug() << "一个worker完成，计数:" << *finish_counter;
        if(*finish_counter >= 2)
        {
            m_btn_start->setEnabled(true);
            delete finish_counter; //计数器用完释放
        }
    };
    connect(m_work_producer, &Worker::signal_finished, this, onWorkerFinished);
    connect(m_work_consumer, &Worker::signal_finished, this, onWorkerFinished);

    //==== 线程结束：断开连接、销毁对象、【最重要：成员指针置空】====
    connect(m_thread_producer, &QThread::finished, this, [this](){
        QObject::disconnect(m_work_producer);
        m_work_producer->deleteLater();
        m_work_producer = nullptr;

        m_thread_producer->deleteLater();
        m_thread_producer = nullptr;
        qDebug() << "生产者线程对象清理完成";
    });

    connect(m_thread_consumer, &QThread::finished, this, [this](){
        QObject::disconnect(m_work_consumer);
        m_work_consumer->deleteLater();
        m_work_consumer = nullptr;

        m_thread_consumer->deleteLater();
        m_thread_consumer = nullptr;
        qDebug() << "消费者线程对象清理完成";
    });

    //跨线程信号槽，默认`Qt::QueuedConnection`，消息投递到主线程事件循环，安全操作 QTextEdit
    m_conn_produce = connect(m_work_producer, &Worker::signal_produce, this, [this](QString msg){
        m_txt_display_area->append(msg);    });
        
    m_conn_consumer = connect(m_work_consumer, &Worker::signal_consumer, this, [this](QString msg){
        m_txt_display_area->append(msg);
    });

    m_btn_start->setEnabled(false);
    m_txt_display_area->clear();

    m_thread_producer->start();
    m_thread_consumer->start();
}

void RightStack024::on_btn_stop()
{
    m_stop_flag = true;
    m_cv.notify_all();
}