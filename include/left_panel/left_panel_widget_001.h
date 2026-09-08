#pragma once

#include <iterator> // IWYU pragma: keep
#include <qpushbutton.h>
#include <qstackedwidget.h>

class LeftStack001: public QWidget
{
    Q_OBJECT
public:
    LeftStack001(QWidget* parent=nullptr);

private:
    void setupUI();
    void connectComponents();

    QPushButton* m_btn_open_right_001{nullptr};
    QPushButton* m_btn_open_right_002{nullptr};
    QPushButton* m_btn_open_right_003{nullptr};  
};