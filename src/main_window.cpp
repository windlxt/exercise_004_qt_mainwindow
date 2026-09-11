#include "main_window.h"
#include "left_panel.h"
#include "right_panel.h"
#include <utility/splitter_custom.h>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <utility/style.h>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setupUI();
    connectComponents();    
}

void MainWindow::setupUI()
{
    setWindowTitle("股票分析系统");
    resize(1300, 700);
    // setStyleSheet(StyleMainWindow);    
    
    // 中心部件
    QWidget* central_widget = new QWidget(this);
    setCentralWidget(central_widget);
    QHBoxLayout* root_layout = new QHBoxLayout(central_widget);
    root_layout->setContentsMargins(0,0,0,0);
    root_layout->setSpacing(0);

    // ActivityBar 左边工具栏
    m_activity_bar = new ActivityBar();
    m_activity_bar->addItem("控件", "控件练习", 0, true);
    m_activity_bar->addItem("算法", "算法", 1, true);
    m_activity_bar->addItem("数据", "数据库管理", 2, true);
    m_activity_bar->addItem("运行", "运行和调试", 3, false);
    m_activity_bar->addItem("扩展", "扩展", 4, false);
    m_activity_bar->addItem("账户", "账户", 10, false);
    m_activity_bar->addItem("管理", "管理", 11, false);    

    // 左侧边面板
    m_left_panel = new LeftPanel();
    m_left_panel->setStyleSheet("background:#252526;");
    m_left_panel->setMinimumWidth(0);
    m_left_panel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Expanding);

    // 右侧边面板
    m_right_panel = new RightPanel();
    m_right_panel->setStyleSheet("background:#1e1e1e;");    
    // m_right_panel->stackWidget()->setCurrentIndex(2); 

    
    // QSplitter 分割条
    m_splitter = new CustomSplitter(Qt::Horizontal);
    m_splitter->setHandleWidth(3);
    m_splitter->setStyleSheet(StyleSplitter);

    m_splitter->addWidget(m_left_panel);
    m_splitter->addWidget(m_right_panel);
    m_splitter->setSizes({m_last_side_width, 1020});
    // m_splitter->setCollapsible(0, true);  // 索引0允许折叠到0    

    // 加入布局
    root_layout->addWidget(m_activity_bar);
    root_layout->addWidget(m_splitter);

    // 动画
    m_anim = new QVariantAnimation(this);
    m_anim->setDuration(200);
    m_anim->setEasingCurve(QEasingCurve::OutCubic);    
}

void MainWindow::connectComponents()
{
    // ActivityBar点击逻辑
    connect(m_activity_bar, &ActivityBar::itemClicked, this, [this](int id){
        if(m_anim_running)
            return;

        QPushButton* btn = m_activity_bar->getButtonById(id);
        if(!btn)
            return;

        // 没有对应页面，直接返回
        if(id >= m_left_panel->stackWidget()->count())
        {
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
            m_left_panel->stackWidget()->setCurrentIndex(id);
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
}