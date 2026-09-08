#pragma once

#include <iterator> // IWYU pragma: keep
#include <qstackedwidget.h>
#include <algorithm/widget_producer_comsumer.h>

class RightStack021: public QWidget
{
    Q_OBJECT
public:
    RightStack021(QWidget* parent=nullptr);
    
private:
    void setupUI();
    void connectComponents();

    winProducerComsumer* m_winProducerComsumer{nullptr};
};