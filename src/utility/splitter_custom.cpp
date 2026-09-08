#include <utility/splitter_custom.h>

AnimatedSplitterHandle::AnimatedSplitterHandle(Qt::Orientation orient, QSplitter *parent)
    : QSplitterHandle(orient, parent),
      m_color(m_normalColor),
      m_lineW(m_normalW)
{
    setAttribute(Qt::WA_Hover);

    // 颜色动画
    m_colorAnim = new QPropertyAnimation(this, "lineColor", this);
    m_colorAnim->setDuration(250);
    m_colorAnim->setEasingCurve(QEasingCurve::OutCubic);

    // 线宽动画
    m_widthAnim = new QPropertyAnimation(this, "lineWidth", this);
    m_widthAnim->setDuration(250);
    m_widthAnim->setEasingCurve(QEasingCurve::OutCubic);
}

void AnimatedSplitterHandle::setLineColor(const QColor &c)
{
    m_color = c;
    update();
}

void AnimatedSplitterHandle::setLineWidth(qreal w)
{
    m_lineW = w;
    update();
}

void AnimatedSplitterHandle::enterEvent(QEnterEvent *event)
{
    QSplitterHandle::enterEvent(event);

    m_colorAnim->stop();
    m_colorAnim->setStartValue(m_color);
    m_colorAnim->setEndValue(m_hoverColor);
    m_colorAnim->start();

    m_widthAnim->stop();
    m_widthAnim->setStartValue(m_lineW);
    m_widthAnim->setEndValue(m_hoverW);
    m_widthAnim->start();
}

void AnimatedSplitterHandle::leaveEvent(QEvent *event)
{
    QSplitterHandle::leaveEvent(event);

    m_colorAnim->stop();
    m_colorAnim->setStartValue(m_color);
    m_colorAnim->setEndValue(m_normalColor);
    m_colorAnim->start();

    m_widthAnim->stop();
    m_widthAnim->setStartValue(m_lineW);
    m_widthAnim->setEndValue(m_normalW);
    m_widthAnim->start();
}

void AnimatedSplitterHandle::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    QPen pen(m_color);
    pen.setWidthF(m_lineW);
    pen.setCapStyle(Qt::FlatCap); // 端点平直，不要圆角延长

    if(orientation() == Qt::Horizontal)
    {
        // 水平QSplitter，竖分割线：从顶部一直画到底部 ✅
        int x = rect().center().x();
        painter.setPen(pen);
        painter.drawLine(QPoint(x, rect().top()), QPoint(x, rect().bottom()));
    }
    else
    {
        // 垂直QSplitter，横分割线：从左画到右
        int y = rect().center().y();
        painter.setPen(pen);
        painter.drawLine(QPoint(rect().left(), y), QPoint(rect().right(), y));
    }
}

CustomSplitter::CustomSplitter(Qt::Orientation o, QWidget *parent)
    :QSplitter(o, parent)
{
    setHandleWidth(6);   // 鼠标拖拽热区宽度
    setChildrenCollapsible(false);
}

QSplitterHandle *CustomSplitter::createHandle()
{
    return new AnimatedSplitterHandle(orientation(), this);
}