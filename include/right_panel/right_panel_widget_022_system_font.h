#pragma once

#include <iterator> // IWYU pragma: keep
#include <qstackedwidget.h>

class QListWidgetItem;
class QListWidget;
class QLabel;
class QComboBox;
class QCheckBox;

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