#include <iterator> // IWYU pragma: keep
#include <qboxlayout.h>
#include <qlabel.h>
#include <qpushbutton.h>
#include <qstackedwidget.h>
#include <algorithm/widget_producer_consumer.h>
#include <qtextedit.h>
#include <right_panel_widget_021_producer_consumer.h>
#include <QScrollArea>

// 生产者消费者代码，以及窗口程序，在 algorithm 文件夹里
// algorithm/widget_producer_consumer

RightStack021::RightStack021(QWidget* parent)
    : QWidget(parent)
{
    setupUI();
    connectComponents();    
}

void RightStack021::setupUI()
{
    QVBoxLayout* layout = new QVBoxLayout();

    //  创建多线程代码简介======================================
    QString profile = R"(多线程实现方式：
    (1) 类头文件声明：
        std::queue<int> m_queue;
        std::mutex m_mtx;
        std::condition_variable m_cv;

        std::atomic<bool> m_running{false};
        std::optional<std::thread> m_producer;
        std::optional<std::thread> m_consumer;

        // 回调函数：产生一条消息，传给UI（会包装成Qt信号）
        using MessageCallback = std::function<void(const std::string&)>;
        void setMessageCallback(MessageCallback cb);
        MessageCallback m_msg_cb;

    (2) 类实现文件里：
        // C++生产者消费者回调，收到消息后发射Qt信号。
        // 纯C++程序线程发射信号，由Qt的UI主线程显示信息。不同线程之间的信息传递。
        m_pc.setMessageCallback([this](const std::string& msg){
            emit sigAppendText(QString::fromStdString(msg));
        });

        m_pc.start();

    （3）start函数里：
        m_producer.emplace(&ProducerConsumer::producer_work, this);
        m_consumer.emplace(&ProducerConsumer::consumer_work, this);
    )";

    // 1. 创建标签
    m_lbl_profile = new QLabel(profile);
    m_lbl_profile->setStyleSheet(R"(
        font-size: 20px;
    )");
    // 关键：让QLabel自适应文本高度，不要固定死高度，否则不会滚动
    m_lbl_profile->setWordWrap(true); // 文字自动换行（长文本必备）
    m_lbl_profile->setAlignment(Qt::AlignTop | Qt::AlignLeft);

    // 2. 创建滚动区域
    m_scroll_profile = new QScrollArea(this);
    m_scroll_profile->setWidget(m_lbl_profile);
    m_scroll_profile->setWidgetResizable(true); // 让内部widget跟随scrollarea宽度变化

    // 3. 设置滚动区域固定可视高度500px（就是你原来想要的可视窗口大小）
    m_scroll_profile->setFixedHeight(350);
    m_scroll_profile->setStyleSheet(R"(
        QScrollArea { border:none; }
        QScrollBar:vertical { width:8px; background:#f1f1f1; }
        QScrollBar::handle:vertical { background:#bbbbbb; border-radius:4px; }
        QScrollBar::handle:vertical:hover { background:#999999; }
    )");
    // ====以上是多线程代码简介===========================


    m_winProducerConsumer = new winProducerConsumer();
    layout->addWidget(m_scroll_profile);
    layout->addWidget(m_winProducerConsumer);
    setLayout(layout);    
}

void RightStack021::connectComponents()
{

}