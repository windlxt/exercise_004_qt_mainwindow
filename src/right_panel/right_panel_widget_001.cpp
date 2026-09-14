#include <iterator> // IWYU pragma: keep
#include <qboxlayout.h>
#include <qbuttongroup.h>
#include <qlabel.h>
#include <qnamespace.h>
#include <qpushbutton.h>
#include <qstackedwidget.h>
#include <right_panel_widget_001.h>
#include <QGroupBox>
#include <QRadioButton>
#include <QTextEdit>
#include <utility/style.h>

RightStack001::RightStack001(QWidget* parent)
    : QWidget(parent)
{
    setupUI();
    connectComponents();
}

void RightStack001::setupUI()
{    
    QGridLayout* layout_root = new QGridLayout(this);    
    // ===== 外边距：layout 和父widget边框之间距离（上下左右）
    layout_root->setContentsMargins(12, 12, 12, 12);
    // 参数顺序：left, top, right, bottom

    // ===== 内边距：单元格之间间距（控件与控件之间）
    layout_root->setHorizontalSpacing(8);   // 水平方向单元格间距
    layout_root->setVerticalSpacing(8);     // 垂直方向单元格间距

    // 1. 组合按钮    
    // ====== 新增：把radio放进QGroupBox ======
    QGroupBox* groupBox = new QGroupBox(tr("学历选择"));      
    QVBoxLayout* groupLayout = new QVBoxLayout(groupBox);

    btnGroup = new QButtonGroup(this);
    QRadioButton *radio1 = new QRadioButton(tr("本科"));
    QRadioButton *radio2 = new QRadioButton(tr("硕士"));
    QRadioButton *radio3 = new QRadioButton(tr("博士"));
    radio1->setChecked(true); // 默认选中本科

    btnGroup->addButton(radio1);
    btnGroup->addButton(radio2);
    btnGroup->addButton(radio3);

    groupLayout->addWidget(radio1);
    groupLayout->addWidget(radio2);
    groupLayout->addWidget(radio3);
    groupLayout->addStretch(1);

    // 2. 文本框
    m_textEdit = new QTextEdit();    
    // m_textEdit->setStyleSheet(StyleTextEdit);

    // 3. 按钮
    QPushButton* m_btn_example = new QPushButton("第一个例子！");
    QLabel* m_lbl_001 = new QLabel("占位标签");    

    layout_root->addWidget(groupBox, 0, 0, 3,1);    
    layout_root->addWidget(m_textEdit, 0, 1, 3,1);
    layout_root->addWidget(m_btn_example, 3, 0, 1, 2);    
    layout_root->addWidget(m_lbl_001, 4,0, 1, 2);

    // 设置拉伸权重
    layout_root->setRowStretch(0, 1);
    layout_root->setRowStretch(1, 1);
    layout_root->setRowStretch(2, 1);
    layout_root->setRowStretch(3, 1);
    layout_root->setRowStretch(4, 2);

    layout_root->setColumnStretch(0, 1);
    layout_root->setColumnStretch(1, 3);        
}

void RightStack001::connectComponents()
{
    connect(btnGroup,&QButtonGroup::buttonClicked,m_textEdit,[this](){
        QAbstractButton* checkedBtn = btnGroup->checkedButton();
        if (checkedBtn != nullptr)
        {
            QString text = checkedBtn->text();
            m_textEdit->setText(text);
            // 如果追加，用 m_textEdit->append(text);
        }
        else
        {
            m_textEdit->setText(tr("未选择"));
        }
    });
}