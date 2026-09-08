#pragma once

#include <iterator> // IWYU pragma: keep
#include <qstackedwidget.h>

class QGroupBox;
class QTextEdit;
class QButtonGroup;

class RightStack001: public QWidget
{
    Q_OBJECT
public:
    RightStack001(QWidget* parent=nullptr);

private:
    void setupUI();
    void connectComponents();

    // 私有成员变量
    QGroupBox *groupBox;
    QTextEdit* m_textEdit{nullptr};
    QButtonGroup *btnGroup{nullptr};
};