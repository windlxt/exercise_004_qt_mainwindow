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

#include <QColor>
#include <QLabel>
#include <QVBoxLayout>

class ColorBlock : public QWidget
{
    Q_OBJECT
public:
    explicit ColorBlock(const QColor& color, QWidget *parent = nullptr);

signals:
    // 左键：设置文字颜色
    void selectTextColor(const QColor& c);
    // 右键：设置背景颜色
    void selectBgColor(const QColor& c);

protected:
    void mousePressEvent(QMouseEvent *event) override;

private:
    QColor m_color;
    QLabel* m_colorLabel{nullptr};
    QLabel* m_textLabel{nullptr};
};

class RightStack027: public QWidget
{
    Q_OBJECT
public:
    RightStack027(QWidget* parent=nullptr);    
    
private:
    void setupUI();
    void connectComponents();
    
private slots:
    void onSelectTextColor(const QColor& c);
    void onSelectBgColor(const QColor& c);
    void updatePreviewStyle();

private:
    void updateRightColorPreview();

    QLabel* m_previewLabel{nullptr};
    QColor m_bgColor;
    QColor m_textColor;

    // ===== 新增右侧面板颜色预览控件 =====
    QLabel* m_textColorPreviewLabel{nullptr};
    QLabel* m_textColorValueLabel{nullptr};
    QLabel* m_bgColorPreviewLabel{nullptr};
    QLabel* m_bgColorValueLabel{nullptr};
};