#include <iterator> // IWYU pragma: keep
#include <qboxlayout.h>
#include <qlabel.h>
#include <qpushbutton.h>
#include <qstackedwidget.h>
#include <algorithm/widget_producer_consumer.h>
#include <qtextedit.h>
#include <right_panel_widget_026_thread_qtimer.h>
#include <QScrollArea>

// 生产者消费者代码，以及窗口程序，在 algorithm 文件夹里
// algorithm/widget_producer_consumer

RightStack026::RightStack026(QWidget* parent)
    : QWidget(parent)
{
    setupUI();
    connectComponents();    
}

RightStack026::~RightStack026()
{
    if(m_worker)
        m_worker->stopWork();
    if(m_thread)
    {
        m_thread->quit();
        m_thread->wait();
    }
}

void RightStack026::setupUI()
{    
    QVBoxLayout* layout = new QVBoxLayout(this);
    m_btnStart = new QPushButton("启动");
    m_btnPause = new QPushButton("暂停");
    m_btnResume = new QPushButton("恢复");
    m_txtLog = new QTextEdit;
    layout->addWidget(m_btnStart);
    layout->addWidget(m_btnPause);
    layout->addWidget(m_btnResume);
    layout->addWidget(m_txtLog);    

    qDebug() << "UI主线程ID:" << QThread::currentThreadId();

    m_thread = new QThread;
    m_worker = new WorkerQTimer;
    m_worker->moveToThread(m_thread);

    // 线程启动后，触发worker.startWork，创建定时器
    connect(m_thread, &QThread::started, m_worker, &WorkerQTimer::startWork);
    // 主线发送暂停信号到worker子线程槽
    connect(this, &RightStack026::sigSetPause, m_worker, &WorkerQTimer::setPause);
    // worker日志信号，回传给UI主线程更新文本框
    connect(m_worker, &WorkerQTimer::logMsg, this, &RightStack026::appendLog);

    // 资源清理
    connect(m_worker, &WorkerQTimer::finished, m_thread, &QThread::quit);
    connect(m_thread, &QThread::finished, m_worker, &WorkerQTimer::deleteLater);
    connect(m_thread, &QThread::finished, m_thread, &QThread::deleteLater);

    connect(m_btnStart, &QPushButton::clicked, this, [this](){
        m_thread->start();
        m_btnStart->setEnabled(false);
    });
    connect(m_btnPause, &QPushButton::clicked, this, [this](){
        emit sigSetPause(true);
    });
    connect(m_btnResume, &QPushButton::clicked, this, [this](){
        emit sigSetPause(false);
    });
}

void RightStack026::appendLog(QString msg)
{
    m_txtLog->append(msg);
}

void RightStack026::onStart()
{
    // 你的业务代码，启动线程
}

void RightStack026::onPause()
{
    // 暂停逻辑
}

void RightStack026::onResume()
{
    // 恢复逻辑
}


void RightStack026::connectComponents()
{

}

