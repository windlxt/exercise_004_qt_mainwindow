
#include <iterator> // IWYU pragma: keep
#include <QApplication>
#include <QPalette>
#include <QStyle>
#include <utility/style.h>
#include <QWidget>

void applyTheme(const QString& theme)
{
    QString style;
    if (theme == "light")
    {
        style = R"(
        *{
            font-family: "Microsoft YaHei";
            font-size: 18px;
            font-weight: normal;            
        }
        /* 全局基础 */
        QWidget{
            background-color: #ffffff;
            color: #000000;
        }
        /* 主窗口菜单 */
        QMenuBar {
            background-color: #f0f0f0;
            color: #000000;
        }
        QMenuBar::item:selected {
            background-color: #cce8ff;
        }
        QMenu {
            background-color: #ffffff;
            color: #000000;
        }
        QMenu::item:selected{
            background-color: #cce8ff;
        }
        /* Dock侧边面板 */
        QDockWidget {
            background-color: #f6f6f6;
            titlebar-close-icon: none;
        }
        QDockWidget::title{
            background-color: #e8e8e8;
            padding:4px;
        }
        /* GroupBox分组框（你的配置窗口） */
        QGroupBox {
            border:1px solid #aaaaaa;
            border-radius:6px;
            margin-top:10px;
            background-color:#ffffff;
        }
        QGroupBox::title {
            subcontrol-origin: margin;
            left:10px;
            padding:0 4px;
            color:#333333;
        }
        /* 输入控件 */
        QLineEdit, QSpinBox, QComboBox{
            background-color:#ffffff;
            border:1px solid #bbbbbb;
            color:#000000;
            min-height:24px;
        }
        QComboBox::drop-down{
            border-left:1px solid #bbbbbb;
        }
        /* 按钮 */
        QPushButton{
            background-color:#f0f0f0;
            border:1px solid #bbbbbb;
            border-radius:4px;
            color:#000000;
            min-width:80px;
            min-height:42px;
        }
        QPushButton:hover{
            background-color:#e0e0e0;
        }
        /* 复选框 */
        QCheckBox{
            background:transparent;
        }
        )";
    }
    else if (theme == "dark")
    {
        style = R"(
        *{
            font-family: "Microsoft YaHei";
            font-size: 18px;
            font-weight: normal;            
        }
        QWidget{
            background-color: #2c2c2c;
            color: #ffffff;
        }
        QMenuBar {
            background-color: #3c3c3c;
            color: #ffffff;
        }
        QMenuBar::item:selected {
            background-color: #486888;
        }
        QMenu {
            background-color: #3c3c3c;
            color: #ffffff;
        }
        QMenu::item:selected{
            background-color: #486888;
        }
        QDockWidget {
            background-color: #343434;
        }
        QDockWidget::title{
            background-color: #444444;
            padding:4px;
        }
        QGroupBox {
            border:1px solid #666666;
            border-radius:6px;
            margin-top:10px;
            background-color:#2c2c2c;
        }
        QGroupBox::title {
            subcontrol-origin: margin;
            left:10px;
            padding:0 4px;
            color:#dddddd;
        }
        QLineEdit, QSpinBox, QComboBox{
            background-color:#444444;
            border:1px solid #666666;
            color:#ffffff;
            min-height:24px;
        }
        QComboBox::drop-down{
            border-left:1px solid #666666;
        }
        QPushButton{
            background-color:#444444;
            border:1px solid #666666;
            border-radius:4px;
            color:#ffffff;
            min-width:80px;
            min-height:42px;
        }
        QPushButton:hover{
            background-color:#555555;
        }
        QCheckBox{
            background:transparent;
        }
        )";
    }
    qApp->setStyleSheet(style);
}


