#pragma once

#include <iterator> // IWYU pragma: keep
#include <qstackedwidget.h>
#include <algorithm/widget_producer_consumer.h>

class RightStack021: public QWidget
{
    Q_OBJECT
public:
    RightStack021(QWidget* parent=nullptr);
    
private:
    void setupUI();
    void connectComponents();

    winProducerConsumer* m_winProducerConsumer{nullptr};
};