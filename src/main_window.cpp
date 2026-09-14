#include "main_window.h"
#include "config.h"
#include "left_panel.h"
#include "right_panel.h"
#include <qapplication.h>
#include <qdebug.h>
#include <qdir.h>
#include <qnamespace.h>
#include <utility/splitter_custom.h>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <utility/style.h>
#include <QMessageBox>
#include <QMenuBar>
#include <utility/config_manager.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setupUI();
    connectComponents();    
}
MainWindow::~MainWindow()
{}

void MainWindow::setupUI()
{
    setWindowTitle("股票分析系统");    
    setFixedSize(1500,750); //设置初始固定大小，防止窗口出现闪动变大 
    
    ////////////////////////////////////////////////////////////
    // 中心部件
    QWidget* central_widget = new QWidget(this);
    setCentralWidget(central_widget);
    QHBoxLayout* root_layout = new QHBoxLayout(central_widget);
    root_layout->setContentsMargins(0,0,0,0);
    root_layout->setSpacing(0);

    // ActivityBar 左边工具栏
    m_activity_bar = new ActivityBar();
    m_activity_bar->setObjectName("activityBar"); // 关键！
    m_activity_bar->addItem("控件", "控件练习", 1, true);   // id 对应打开的左侧面板
    m_activity_bar->addItem("算法", "算法", 2, true);
    m_activity_bar->addItem("数据", "数据库管理", 3, true);
    m_activity_bar->addItem("运行", "运行和调试", 4, false);
    m_activity_bar->addItem("扩展", "扩展", 5, false);
    m_activity_bar->addItem("账户", "账户", 10, false);
    m_activity_bar->addItem("管理", "管理", 11, false);
    m_left_panel_id_set = {1, 2, 3, 4, 5, 10, 11}; 
    m_activity_bar->getButtonById(1)->setChecked(true); //第1个按钮显示选中状态

    // 左侧边面板    
    m_left_panel = new LeftPanel();    
    m_left_panel->setMinimumWidth(0);
    m_left_panel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Expanding);
    
    // 右侧边面板
    m_right_panel = new RightPanel();    

    
    // QSplitter 分割条
    m_splitter = new CustomSplitter(Qt::Horizontal);
    m_splitter->setHandleWidth(3);

    m_splitter->addWidget(m_left_panel);
    m_splitter->addWidget(m_right_panel);
    m_splitter->setSizes({m_last_side_width, 1020});
    // m_splitter->setCollapsible(0, true);  // 索引0允许折叠到0    

    // 加入布局
    root_layout->addWidget(m_activity_bar);
    root_layout->addWidget(m_splitter);

    //// ========== 构建菜单栏 ==========
    m_menuBar = this->menuBar();

    // 文件菜单
    QMenu* menuFile = m_menuBar->addMenu("文件(&F)");
    actOpen = menuFile->addAction("打开(&O)");
    actSave = menuFile->addAction("保存(&S)");
    menuFile->addSeparator(); // 分隔线
    actExit = menuFile->addAction("退出(&X)");

    // 主题菜单
    QMenu* menuConfig = m_menuBar->addMenu("配置(&T)");    
    actLight = menuConfig->addAction("亮色");
    actDark = menuConfig->addAction("暗色");
    menuConfig->addSeparator(); // 分隔线
    actSetConfig = menuConfig->addAction("配置...");

    // 帮助菜单
    QMenu* menuHelp = m_menuBar->addMenu("帮助(&H)");
    actAbout = menuHelp->addAction("关于");

    ////////////////////////////////////////////////////////////////
    // 动画
    m_anim = new QVariantAnimation(this);
    m_anim->setDuration(200);
    m_anim->setEasingCurve(QEasingCurve::OutCubic); 

}

void MainWindow::connectComponents()
{
    // 预装载信号连接
    connect(this, &MainWindow::preload_panel, m_left_panel,&LeftPanel::preload_left_stack_widget, Qt::QueuedConnection);
    connect(this, &MainWindow::preload_panel, m_right_panel,&RightPanel::preload_right_stack_widget, Qt::QueuedConnection);

    ////////////////////////////////////////////////////////
    // ActivityBar点击逻辑
    connect(m_activity_bar, &ActivityBar::itemClicked, this, [this](int id){
        if(m_anim_running)
            return;

        QPushButton* btn = m_activity_bar->getButtonById(id);
        if(!btn)
            return;

        // 没有对应页面，直接返回
        if(!m_left_panel_id_set.contains(id))
        {
            qDebug()<<"没有对应页面，直接返回";
            return;
        }

        if(m_activePanelId == id)
        {
            // 点击当前激活按钮：折叠到0
            btn->setChecked(false);
            m_activePanelId = -1;

            QList<int> sz = m_splitter->sizes();
            m_last_side_width = sz.size()>=1 ? sz[0] : m_last_side_width;

            m_anim->setStartValue(sz.size()>=1 ? sz[0] : m_last_side_width);
            m_anim->setEndValue(0);
            m_anim->start();
            m_anim_running = true;
            m_splitter->setEnabled(false);
        }
        else
        {
            // 切换新面板
            if(m_activePanelId != -1)
            {
                QPushButton* oldBtn = m_activity_bar->getButtonById(m_activePanelId);
                if(oldBtn)
                {
                    oldBtn->setChecked(false);
                }
            }
            btn->setChecked(true);
            m_activePanelId = id;

            if(!m_side_visible)
            {
                m_anim->setStartValue(0);
                m_anim->setEndValue(m_last_side_width);
                m_anim->start();
                m_anim_running = true;
                m_splitter->setEnabled(false);
            }
            // m_left_panel->stackWidget()->setCurrentIndex(id);
            m_left_panel->open_id_left_stack_widget(id);

            m_side_visible = true;
        }
    });

    // 监听拖拽分割条，保存宽度
    connect(m_splitter, &QSplitter::splitterMoved, this, [this](int pos, int index){
        // 方法1：
        // QList<int> sz = m_splitter->sizes();
        // if(sz.size() >= 2)
        // {
        //     m_last_side_width = sz[0];
        // }
        
        // 方法2：
        if(index ==0 && !m_anim_running)
        {
            m_last_side_width = pos;
        }
    });

    // 动画每一帧更新splitter尺寸
    connect(m_anim, &QVariantAnimation::valueChanged, this, [this](const QVariant& val){
        if(!m_splitter) return;
        int w = val.toInt();
        QList<int> sizes = m_splitter->sizes();
        if(sizes.size() >= 2)
        {
            sizes[0] = w;
            m_splitter->setSizes(sizes);
        }
    });

    // 动画结束回调，仅更新标记，不show/hide控件
    connect(m_anim, &QVariantAnimation::finished, this, [this](){
        m_anim_running = false;
        if(m_splitter)
            m_splitter->setEnabled(true);

        QList<int> sz = m_splitter->sizes();
        int curW = sz.size() >= 1 ? sz[0] : 0;
        if(curW <= 0)
        {            
            m_side_visible = false;
        }
        else
        {            
            m_side_visible = true;
        }
    });
    ///////////////////////////////////////////////

    // 菜单相关
    // ========== 绑定Action槽函数 ==========
    // 配置窗口
    connect(actSetConfig, &QAction::triggered, [this](){
        m_config_window = new ConfigGlobal(this);           
        m_config_window->setAttribute(Qt::WA_DeleteOnClose);
        m_config_window->setWindowFlags(Qt::Window);    // 关键：设置为顶层独立窗口
        m_config_window->show();
        m_config_window->raise();
        m_config_window->activateWindow();
    });
    connect(actLight, &QAction::triggered, [this](){
        applyTheme(QString("light"));
        
    });
    connect(actDark, &QAction::triggered, [this](){
        applyTheme(QString("dark"));
    });

    connect(actExit, &QAction::triggered, this, &QMainWindow::close);
    connect(actAbout, &QAction::triggered, this, [this](){
        QMessageBox::about(this, "关于", "学习 Qt6 程序");
    });

    // 监听主题变更
    connect(ConfigManager::getInstance(), &ConfigManager::sigConfigChanged,
            this, [this](const GlobalConfigData& cfg){
                QString theme = cfg.themeMode;

                qDebug() << "收到主题变更:" << cfg.themeMode;
                applyTheme(cfg.themeMode);
            });
}
