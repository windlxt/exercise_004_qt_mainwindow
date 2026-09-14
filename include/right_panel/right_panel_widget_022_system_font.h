#pragma once

#include <iterator> // IWYU pragma: keep
#include <qnamespace.h>
#include <qstackedwidget.h>
#include <QStyledItemDelegate>

class QListWidgetItem;
class QListWidget;
class QLabel;
class QComboBox;
class QCheckBox;


//不每个 item 单独存 font，绘制时动态构造字体，减少内存开销，适合几百上千字体
class FontDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit FontDelegate(QObject *parent = nullptr)
        : QStyledItemDelegate(parent)
    {}

    void paint(QPainter *painter,
               const QStyleOptionViewItem &option,
               const QModelIndex &index) const override
    {
        QString family = index.data(Qt::DisplayRole).toString();
        QFont& font = m_fontCache[family];
        if(font.family().isEmpty())
        {
            font = QFont(family,12);
        }

        QStyleOptionViewItem opt = option;
        opt.font = font;

        // 字体回退则置灰色
        if(font.family() != family)
        {
            opt.palette.setColor(QPalette::Text, Qt::lightGray);
        }
        QStyledItemDelegate::paint(painter, opt, index);
    }

    QSize sizeHint(const QStyleOptionViewItem &option,
                   const QModelIndex &index) const override
    {
        QString family = index.data(Qt::DisplayRole).toString();
        QFont& font = m_fontCache[family];
        if(font.family().isEmpty())
        {
            font = QFont(family,12);
        }
        QStyleOptionViewItem opt = option;
        opt.font = font;
        return QStyledItemDelegate::sizeHint(opt, index);
    }

private:
    mutable QHash<QString, QFont> m_fontCache;
};


class RightStack022: public QWidget
{
    Q_OBJECT
public:
    RightStack022(QWidget* parent=nullptr);
    
private:
    void setupUI();
    void connectComponents();

    void loadFontList(bool onlyChinese = false);

private slots:
    void onFontSelected(QListWidgetItem* item);
    void onSizeChanged(int idx);
    void onBoldToggled(bool checked);
    void onItalicToggled(bool checked);
    void onFilterChanged(int idx);

private:
    QListWidget* m_fontList;
    QLabel* m_previewLabel;
    QComboBox* m_cmbSize;
    QComboBox* m_cmbFilter;
    QCheckBox* m_chkBold;
    QCheckBox* m_chkItalic;

    QString m_curFamily;
    int m_curPointSize = 24;
    bool m_bold = false;
    bool m_italic = false;    
};