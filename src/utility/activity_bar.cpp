#include "utility/activity_bar.h"
#include <QFont>
#include <utility/style.h>

ActivityBar::ActivityBar(QWidget *parent)
    : QWidget(parent)
{
    setFixedWidth(48);
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);

    QVBoxLayout* main_lay = new QVBoxLayout(this);
    main_lay->setContentsMargins(0,8,0,8);
    main_lay->setSpacing(0);

    m_layout_top = new QVBoxLayout();
    m_layout_top->setSpacing(0);
    m_layout_bottom = new QVBoxLayout();
    m_layout_bottom->setSpacing(0);

    m_btn_panel_group = new QButtonGroup(this);
    m_btn_panel_group->setExclusive(false);

    m_btn_bottom_group = new QButtonGroup(this);

    main_lay->addLayout(m_layout_top);
    main_lay->addStretch(1);
    main_lay->addLayout(m_layout_bottom);

    setStyleSheet(StyleActivityBar);

    connect(m_btn_panel_group, &QButtonGroup::buttonClicked, this, [this](QAbstractButton* btn){
        int id = m_btn_panel_group->id(btn);
        emit itemClicked(id);
    });
    connect(m_btn_bottom_group, &QButtonGroup::buttonClicked, this, [this](QAbstractButton* btn){
        int id = m_btn_bottom_group->id(btn);
        emit itemClicked(id);
    });
}

void ActivityBar::addItem(const QString &iconText, const QString &tip, int id, bool checkable)
{
    QPushButton* btn = new QPushButton(iconText, this);
    btn->setCheckable(checkable);
    btn->setToolTip(tip);
    m_id_to_btn.insert(id, btn);

    if(checkable)
    {
        m_btn_panel_group->addButton(btn, id);
        m_layout_top->addWidget(btn);
    }
    else
    {
        m_btn_bottom_group->addButton(btn, id);
        m_layout_bottom->addWidget(btn);
    }
}

QPushButton* ActivityBar::getButtonById(int id)
{
    return m_id_to_btn.value(id, nullptr);
}