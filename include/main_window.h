#pragma once

#include <iterator> // IWYU pragma: keep
#include "utility/activity_bar.h"
#include <QStackedWidget>
#include <QSplitter>
#include <QVariantAnimation>
#include <qmainwindow.h>

class LeftPanel;
class RightPanel;

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    
private:
    void setupUI();
    void connectComponents();

    // 私有成员变量
    ActivityBar* m_activity_bar{nullptr};
    LeftPanel* m_left_panel{nullptr};
    QSplitter* m_splitter{nullptr};
    RightPanel* m_right_panel{nullptr};

    bool m_side_visible = true;
    int m_last_side_width = 280;

    QVariantAnimation* m_anim{nullptr};
    bool m_anim_running = false;
    int m_activePanelId = -1; // 记录当前激活的面板id，-1无选中
};