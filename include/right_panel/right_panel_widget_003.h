#pragma once

#include <iterator> // IWYU pragma: keep
#include <qstackedwidget.h>

class RightStack003: public QWidget
{
    Q_OBJECT
public:
    RightStack003(QWidget* parent=nullptr);

private:
    void setupUI();
    void connectComponents();
};