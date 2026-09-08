#pragma once

#include <iterator> // IWYU pragma: keep
#include <qstackedwidget.h>

class LeftStack003: public QWidget
{
    Q_OBJECT
public:
    LeftStack003(QWidget* parent=nullptr);

private:
    void setupUI();
    void connectComponents();
};