#pragma once

#include <iterator> // IWYU pragma: keep
#include <qabstractspinbox.h>
#include <qpushbutton.h>
#include <qstackedwidget.h>
#include <qstyle.h>

class QTextEdit;
class QLabel;
class QLineEdit;
class QScrollArea;

class RightStack025: public QWidget
{
    Q_OBJECT
public:
    RightStack025(QWidget* parent=nullptr);
    ~RightStack025() override;
    
private:
    void setupUI();
    void connectComponents(); 
     
};