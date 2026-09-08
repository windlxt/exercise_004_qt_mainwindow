#pragma once

#include <iterator> // IWYU pragma: keep
#include <QWidget>
#include <QVBoxLayout>
#include <QPushButton>
#include <QButtonGroup>
#include <QHash>

class ActivityBar : public QWidget
{
    Q_OBJECT
public:
    explicit ActivityBar(QWidget *parent = nullptr);
    void addItem(const QString &iconText, const QString &tip, int id, bool checkable = true);
    QPushButton* getButtonById(int id);

signals:
    void itemClicked(int id);

private:
    QVBoxLayout* m_layout_top{nullptr};
    QVBoxLayout* m_layout_bottom{nullptr};
    QButtonGroup* m_btn_panel_group{nullptr};
    QButtonGroup* m_btn_bottom_group{nullptr};
    QHash<int, QPushButton*> m_id_to_btn;
};


