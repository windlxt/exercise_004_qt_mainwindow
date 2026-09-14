#pragma once

#include <iterator> // IWYU pragma: keep
#include <QSplitter>
#include <QSplitterHandle>
#include <QPainter>
#include <QPropertyAnimation>

class AnimatedSplitterHandle : public QSplitterHandle
{
    Q_OBJECT
    Q_PROPERTY(QColor lineColor READ lineColor WRITE setLineColor)
    Q_PROPERTY(qreal lineWidth READ lineWidth WRITE setLineWidth)
public:
    AnimatedSplitterHandle(Qt::Orientation orient, QSplitter* parent);

    QColor lineColor() const { return m_color; }
    void setLineColor(const QColor& c);

    qreal lineWidth() const { return m_lineW; }
    void setLineWidth(qreal w);

protected:
    void enterEvent(QEnterEvent *event) override;
    void leaveEvent(QEvent *event) override;
    void paintEvent(QPaintEvent *event) override;

private:
    const qreal m_normalW = 2.0;
    const qreal m_hoverW = 5.0;
    const QColor m_normalColor{0x44,0x44,0x44};
    const QColor m_hoverColor{0x71,0xa1,0xef};
    QColor m_color;
    qreal m_lineW = 1.0;
    QPropertyAnimation* m_colorAnim;
    QPropertyAnimation* m_widthAnim;
    
    
};

class CustomSplitter : public QSplitter
{
    Q_OBJECT
public:
    explicit CustomSplitter(Qt::Orientation o, QWidget* parent = nullptr);
protected:
    QSplitterHandle* createHandle() override;
};