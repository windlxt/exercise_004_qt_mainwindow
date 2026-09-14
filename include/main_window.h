#pragma once

#include <iterator> // IWYU pragma: keep
#include "utility/activity_bar.h"
#include <QStackedWidget>
#include <QSplitter>
#include <QVariantAnimation>
#include <qmainwindow.h>
#include <QSet>
#include <utility/config.h>

class LeftPanel;
class RightPanel;

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

signals:
    void preload_panel();
    
private:
    void setupUI();
    void connectComponents();

    // 私有成员变量
    ActivityBar* m_activity_bar{nullptr};   // 左侧自定义的工具栏
    LeftPanel* m_left_panel{nullptr};       // 左侧面板
    QSplitter* m_splitter{nullptr};         // 分割条容器
    RightPanel* m_right_panel{nullptr};     // 右侧面板 
    ConfigGlobal* m_config_window{nullptr}; // 配置窗口

    bool m_side_visible = true;
    int m_last_side_width = 280;

    QVariantAnimation* m_anim{nullptr};
    bool m_anim_running = false;
    int m_activePanelId = 1; // 记录当前激活的面板id，-1无选中

    QSet<int> m_left_panel_id_set; // 左侧面板id集合

    // 菜单栏
    QMenuBar* m_menuBar{nullptr};        
    QAction* actOpen;
    QAction* actSave;    
    QAction* actExit;
    QAction* actSetConfig;
    QAction* actLight;
    QAction* actDark;
    QAction* actAbout;
};