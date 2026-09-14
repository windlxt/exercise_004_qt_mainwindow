#include <iterator> // IWYU pragma: keep
#include <qboxlayout.h>
#include <qcontainerfwd.h>
#include <qframe.h>
#include <qlineedit.h>
#include <qnamespace.h>
#include <qpushbutton.h>
#include <qstackedwidget.h>
#include <qtextedit.h>
#include <right_panel_widget_023_time_decorator.h>
#include <QLabel>
#include <QLineEdit>
#include <QFileDialog>
#include <QFile>
#include <QCoreApplication>
#include <utility/global_const_function.h>
#include <chrono>
#include <thread>
#include <QScrollArea>


RightStack023::RightStack023(QWidget* parent)
    : QWidget(parent)
{
    setupUI();
    connectComponents();
}

RightStack023::~RightStack023()
{
    // 设置停止标记，通知子线程退出循环
    m_stop_flag_1.store(true);
    m_stop_flag_2.store(true);

    // 窗口销毁，等待线程安全退出，防止崩溃
    if(m_work_thread_1.joinable())
    {
        m_work_thread_1.join();
        qDebug() << "主窗口关闭，子线程1已经安全回收";
    }

    if(m_work_thread_2.joinable())
    {
        m_work_thread_2.join();
        qDebug() << "主窗口关闭，子线程2已经安全回收";
    }
}

void RightStack023::setupUI()
{       
    QVBoxLayout* layout = new QVBoxLayout(this);

    // 1.打开文件对话框，显示文件路径和内容
    QHBoxLayout* layout_read_file = new QHBoxLayout();
    m_btn_read_src = new QPushButton("文件");
    m_btn_read_src->setFixedSize(150,40);
    m_le_file_path = new QLineEdit();
    layout_read_file->addWidget(m_btn_read_src);
    layout_read_file->addWidget(m_le_file_path);

    m_txt_code = new QTextEdit();
    m_txt_code->setFixedHeight(80);
    m_txt_code->setTextColor(Qt::darkGreen);    

    //  创建多线程代码简介======================================
    QString profile = R"(多线程实现方式：
    在一个类里实现work工作函数，另一个成员函数直接 std::thread(f, this, 1000);
    (1) 类头文件声明：
        void work(int n);   // 工作线程，定义为Qt类的成员函数
        sstd::thread m_work_thread;
        std::atomic<bool> m_stop_flag{false};

    (2) 类实现文件里：
        // 给work加上装饰器，也可以不加
        auto f = time_decorator(&RightStack023::work);
        // 启动子线程，传入主线程中的this指针作为成员函数对象
        m_work_thread = std::thread(f, this, 1000);

    （3）第一种实现方式：work函数里：
        // 任务结束，向主线程发送一条信息，this是主线程传递过来的，修改UI
        QMetaObject::invokeMethod(this, [this](){ 
            // 任务跑完，join回收资源，joinable变为false
            if(m_work_thread.joinable())
            {
                m_work_thread.join();
                qDebug() << "线程资源已回收";
            }       
            m_btn_start_thread->setEnabled(true);
            m_btn_start_thread->setStyleSheet("");        
        }, Qt::QueuedConnection);
    （4）第2种实现方式：work_signal函数里：
            emit msg_to_ui(msg);
        在连接函数中：
        // 绑定signal信号槽2
        connect(this, &RightStack023::msg_to_ui,this, [this](QString msg){
                if(msg != "over")
                {
                    m_txt_work_result->append(msg);
                }                    
                else
                {
                    m_txt_work_result->append(msg);                    
                    
                    // 任务跑完，join回收资源，joinable变为false
                    if(m_work_thread_2.joinable())
                    {
                        m_work_thread_2.join();
                        qDebug() << "线程2资源已回收";
                    }  

                    m_btn_start_thread_2->setEnabled(true);
                    m_btn_start_thread_2->setStyleSheet("");
                }                
            });    

    )";
    

    // 1. 创建标签
    m_lbl_profile = new QLabel(profile);

    // m_lbl_profile->setStyleSheet(R"(
    //     font-size: 20px;
    // )");

    // 关键：让QLabel自适应文本高度，不要固定死高度，否则不会滚动
    m_lbl_profile->setWordWrap(true); // 文字自动换行（长文本必备）
    m_lbl_profile->setAlignment(Qt::AlignTop | Qt::AlignLeft);

    // 2. 创建滚动区域
    m_scroll_profile = new QScrollArea(this);
    m_scroll_profile->setWidget(m_lbl_profile);
    m_scroll_profile->setWidgetResizable(true); // 让内部widget跟随scrollarea宽度变化

    // 3. 设置滚动区域固定可视高度500px（就是你原来想要的可视窗口大小）
    m_scroll_profile->setFixedHeight(250);
    // m_scroll_profile->setStyleSheet(R"(
    //     QScrollArea { border:none; }
    //     QScrollBar:vertical { width:8px; background:#f1f1f1; }
    //     QScrollBar::handle:vertical { background:#bbbbbb; border-radius:4px; }
    //     QScrollBar::handle:vertical:hover { background:#999999; }
    // )");
    // ====以上是多线程代码简介===========================

    // 3.程序运行时间展示
    m_lb_time = new QLabel("程序运行时间：");    
    // m_lb_time->setStyleSheet(R"(QLabel{
    //     color: #ffffff;
    //     font-size:30px;
    //     background-color:transparent;
    // }
    // )");

    m_le_time_result = new QLineEdit();
    // m_le_time_result->setStyleSheet(R"(QLineEdit{
    //     color: #ffffff;
    //     font-size:30px;
    //     background-color:gray;
    //     border:1px solid #444444;
    //     border-radius:8px;
    //     padding: 6px 12px;
    // }
    // )");
    m_le_time_result->setPlaceholderText("因使用了计时装饰器，无法在主窗口线程获取运行时间");
    
    m_btn_start_thread_1 = new QPushButton("方式一：开始新的线程");

    ///////一条带颜色的横线////////////////////////////////////
    QFrame* line1 = new QFrame(this);
    line1->setFrameShape(QFrame::HLine);    // 水平线条
    line1->setFrameShadow(QFrame::Plain);
    // line1->setStyleSheet(R"(
    //     QFrame {
    //         background-color: #0b9a5f;
    //         min-height:1px;
    //         max-height:1px;
    //     }
    // )");   
    ///////第二条带颜色的横线////////////////////////////////////
    QFrame* line2 = new QFrame(this);
    line2->setFrameShape(QFrame::HLine);    // 水平线条
    line2->setFrameShadow(QFrame::Plain);
    // line2->setStyleSheet(R"(
    //     QFrame {
    //         border: none;
    //         border-top: 1px solid #0b9a5f;
    //         min-height:1px;
    //         max-height:1px;
    //     }
    // )");
    ///////第二条带颜色的横线////////////////////////////////////
    QFrame* line3 = new QFrame(this);
    line3->setFrameShape(QFrame::HLine);    // 水平线条
    line3->setFrameShadow(QFrame::Plain);
    // line3->setStyleSheet(R"(
    //     QFrame {
    //         border: none;
    //         border-top: 1px solid #0b9a5f;
    //         min-height:1px;
    //         max-height:1px;
    //     }
    // )");

    // 第二种方式，用信号槽多次传递信息////////////////////////
    QHBoxLayout* layout_2 = new QHBoxLayout();
    m_txt_work_result = new QTextEdit();
    // m_txt_work_result->setStyleSheet(R"(
    //     QTextEdit {
    //         background-color: #76c7e8;
    //         color: #f24425;
    //     }
    // )");
    m_btn_start_thread_2 = new QPushButton("方式二：开始新的线程[信号槽]");
    // m_btn_start_thread_2->setStyleSheet(R"(
    //     QPushButton {
    //         padding: 4px 8px;            
    //     }
    //     QPushButton:hover {
    //         background-color:#e8e8e8;
    //     }
    //     QPushButton:disabled {
    //         color:#999;
    //         background-color:#eeeeee;
    //     }
    // )");

    layout_2->addWidget(m_txt_work_result);
    layout_2->addWidget(m_btn_start_thread_2);
    //////////////////////////////////////////////// 

    layout->addLayout(layout_read_file);
    layout->addWidget(m_txt_code);
    layout->addWidget(line1);    
    layout->addWidget(m_scroll_profile);
    layout->addWidget(line2);
    layout->addWidget(m_lb_time);
    layout->addWidget(m_le_time_result);
    layout->addWidget(m_btn_start_thread_1);
    layout->addWidget(line3);
    // layout->addStretch();
    layout->addLayout(layout_2);
    
    layout->setSpacing(5); // 控件之间的间隔
    layout->setContentsMargins(10,10,10,10);// 左上右下      
}

//点击读取文件按钮槽函数
void RightStack023::on_read_src_clicked()
{
    // 弹出选择单个文件对话框
    QString filePath = QFileDialog::getOpenFileName(
        this,
        "选择文件",                    // 对话框标题
        QCoreApplication::applicationDirPath(), // exe所在目录,             // 默认打开目录：用户主目录
        "程序文件 (*.h *.cpp);;所有文件 (*.*)"  // 文件过滤器
    );

    // 用户点取消，返回空字符串
    if (filePath.isEmpty())
    {
        qDebug() << "用户取消选择";
        return;
    }

    // 读取选中的文件
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << "文件打开失败：" << file.errorString();
        return;
    }

    QTextStream stream(&file);
    stream.setEncoding(QStringConverter::Utf8); // utf8中文
    QString content = stream.readAll();

    file.close();
    m_le_file_path->setText(filePath);
    qDebug() << "已读取文件：" << filePath;
    m_txt_code->clear();
    m_txt_code->append(content);    
}


// 线程的工作函数,用QMetaObject::invokeMethod，发送1次消息
void RightStack023::work(int n)
{
    for(int i = 0; i < n; ++i)
    {
        if(m_stop_flag_1.load())
        {
            qDebug() << "子线程收到停止信号，提前退出";
            break;
        }
        qDebug() << "正在运行:" << i;
        // chrono标准休眠
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
    qDebug() << "运行结束。";   
    
    // 任务结束，切回主线程恢复按钮
    QMetaObject::invokeMethod(this, [this](){ 
        // 任务跑完，join回收资源，joinable变为false
        if(m_work_thread_1.joinable())
        {
            m_work_thread_1.join();
            qDebug() << "线程1资源已回收";
        }  

        m_btn_start_thread_1->setEnabled(true);
        // m_btn_start_thread_1->setStyleSheet("");        
    }, Qt::QueuedConnection);
}

void RightStack023::on_start_thread_1()
{
    // 防止重复点击启动多个线程
    if(m_work_thread_1.joinable())
    {
        qDebug() << "任务正在运行中，请勿重复点击";
        return;
    }
    m_stop_flag_1.store(false);   // 启动任务，重置停止标记

    // 给work加上装饰器，也可以不加
    auto f = time_decorator(&RightStack023::work);
    // 启动子线程，传入this指针作为成员函数对象
    m_work_thread_1 = std::thread(f, this, 1000);

    m_btn_start_thread_1->setEnabled(false);
    // m_btn_start_thread_1->setStyleSheet("color:gray");
}

//第二种方式，用信号槽实现多次传递信息////////////////////
// 线程的工作函数
void RightStack023::work_signal(int n)
{
    for(int i = 0; i < n; ++i)
    {
        if(m_stop_flag_2.load())
        {
            qDebug() << "子线程收到停止信号，提前退出";
            break;
        }        
        // emit msg_to_ui(QString::fromStdString(std::format("正在运行:{:03d}", i)));
        emit msg_to_ui(QString("正在运行:%1").arg(i, 3, 10, '0'));
        // chrono标准休眠
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
    qDebug() << "运行结束。"; 
    emit msg_to_ui("over");  
}

void RightStack023::on_start_thread_2()
{
    // 防止重复点击启动多个线程
    if(m_work_thread_2.joinable())
    {
        qDebug() << "任务正在运行中，请勿重复点击";
        return;
    }
    m_stop_flag_2.store(false);   // 启动任务，重置停止标记

    // 给work加上装饰器，也可以不加
    auto f = time_decorator(&RightStack023::work_signal);
    // 启动子线程，传入this指针作为成员函数对象
    m_work_thread_2 = std::thread(f, this, 500);

    m_btn_start_thread_2->setEnabled(false);
    // m_btn_start_thread_2->setStyleSheet("color:gray");
}

////////////////////////////////////////////////////////


// 信号槽连接
void RightStack023::connectComponents()
{
    // 绑定文件点击信号槽
    connect(m_btn_read_src, &QPushButton::clicked,
            this, &RightStack023::on_read_src_clicked);
    
    // 绑定开始按钮信号槽1
    connect(m_btn_start_thread_1, &QPushButton::clicked,
            this, &RightStack023::on_start_thread_1);

    // 绑定开始按钮信号槽2
    connect(m_btn_start_thread_2, &QPushButton::clicked,
            this, &RightStack023::on_start_thread_2);

    // 绑定signal信号槽2
    connect(this, &RightStack023::msg_to_ui,
            this, [this](QString msg){
                if(msg != "over")
                {
                    m_txt_work_result->append(msg);
                }                    
                else
                {
                    m_txt_work_result->append(msg);                    
                    
                    // 任务跑完，join回收资源，joinable变为false
                    if(m_work_thread_2.joinable())
                    {
                        m_work_thread_2.join();
                        qDebug() << "线程2资源已回收";
                    }  

                    m_btn_start_thread_2->setEnabled(true);
                    // m_btn_start_thread_2->setStyleSheet("");
                }                
            });    
}
