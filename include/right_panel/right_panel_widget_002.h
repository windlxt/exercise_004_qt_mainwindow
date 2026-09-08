#pragma once

#include <iterator> // IWYU pragma: keep
#include <qstackedwidget.h>

class RightStack002: public QWidget
{
    Q_OBJECT
public:
    RightStack002(QWidget* parent=nullptr);
    
private:
    void setupUI();
    void connectComponents();
};