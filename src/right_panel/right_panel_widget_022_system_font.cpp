#include <iterator> // IWYU pragma: keep
#include <qboxlayout.h>
#include <qpushbutton.h>
#include <qstackedwidget.h>
#include <right_panel_widget_022_system_font.h>
#include <QListWidget>
#include <QLabel>
#include <QComboBox>
#include <QCheckBox>
#include <QFontDatabase>


RightStack022::RightStack022(QWidget* parent)
    : QWidget(parent)
{
    setupUI();    
    loadFontList(false);
    connectComponents();
}

void RightStack022::setupUI()
{    
    QHBoxLayout* layout_root = new QHBoxLayout(this);

    // 左侧布局：字体列表 + 筛选
    QVBoxLayout* leftLayout = new QVBoxLayout();
    m_cmbFilter = new QComboBox();
    m_cmbFilter->addItems({"全部字体", "仅简体中文字体"});
    m_fontList = new QListWidget();
    // 设置 FontDelegate
    m_fontList->setItemDelegate(new FontDelegate);

    leftLayout->addWidget(m_cmbFilter);
    leftLayout->addWidget(m_fontList);

    // 右侧布局：预览+字号+粗斜体
    QVBoxLayout* rightLayout = new QVBoxLayout();
    QHBoxLayout* optLayout = new QHBoxLayout();
    m_cmbSize = new QComboBox();
    QList<int> stdSizes = QFontDatabase::standardSizes();
    for(int s : stdSizes){
        m_cmbSize->addItem(QString::number(s), s);
    }
    m_cmbSize->setCurrentText("24");
    m_chkBold = new QCheckBox("粗体");
    m_chkItalic = new QCheckBox("斜体");
    optLayout->addWidget(new QLabel("字号："));
    optLayout->addWidget(m_cmbSize);
    optLayout->addWidget(m_chkBold);
    optLayout->addWidget(m_chkItalic);
    optLayout->addStretch();

    m_previewLabel = new QLabel("预览文字\nChina is a great country\n天地玄黄，宇宙洪荒");
    m_previewLabel->setAlignment(Qt::AlignCenter);
    // m_previewLabel->setStyleSheet("border:1px solid #aaa; padding:20px;");
    rightLayout->addLayout(optLayout);
    rightLayout->addWidget(m_previewLabel);
    rightLayout->addStretch();

    layout_root->addLayout(leftLayout, 1);
    layout_root->addLayout(rightLayout,1);
}

void RightStack022::loadFontList(bool onlyChinese)
{
    m_fontList->clear();
    QStringList families;
    if(onlyChinese){
        families = QFontDatabase::families(QFontDatabase::SimplifiedChinese);
    }else{
        families = QFontDatabase::families();
    }
    families.removeDuplicates();
    families.sort(Qt::CaseInsensitive);
    for(const QString& family : families)
    {
        // QListWidgetItem* item = new QListWidgetItem(family, m_fontList);
        // QFont font(family, 12);
        // item->setFont(font);

        //只需添加字体字符串，等绘制时，再利用 FontDelegate 生成字体。
        new QListWidgetItem(family, m_fontList);
    }
}

// slot function
void RightStack022::onFilterChanged(int idx)
{
    if(idx == 1)
        loadFontList(true);
    else
        loadFontList(false);
}

// slot function
void RightStack022::onFontSelected(QListWidgetItem *item)
{
    if(!item) return;
    m_curFamily = item->text();
    QFont previewFont(m_curFamily, m_curPointSize);
    previewFont.setBold(m_bold);
    previewFont.setItalic(m_italic);
    m_previewLabel->setFont(previewFont);

    //打印字体附加信息
    QStringList styles = QFontDatabase::styles(m_curFamily);
    qDebug() << "选中字体：" << m_curFamily << "可用样式:" << styles;
}

// slot function
void RightStack022::onSizeChanged(int idx)
{
    m_curPointSize = m_cmbSize->currentData().toInt();
    if(!m_curFamily.isEmpty()){
        QFont previewFont(m_curFamily, m_curPointSize);
        previewFont.setBold(m_bold);
        previewFont.setItalic(m_italic);
        m_previewLabel->setFont(previewFont);
    }
}

// slot function
void RightStack022::onBoldToggled(bool checked)
{
    m_bold = checked;
    if(!m_curFamily.isEmpty()){
        QFont f = m_previewLabel->font();
        f.setBold(m_bold);
        m_previewLabel->setFont(f);
    }
}

// slot function
void RightStack022::onItalicToggled(bool checked)
{
    m_italic = checked;
    if(!m_curFamily.isEmpty()){
        QFont f = m_previewLabel->font();
        f.setItalic(m_italic);
        m_previewLabel->setFont(f);
    }
}

// 信号槽连接
void RightStack022::connectComponents()
{
    connect(m_fontList, &QListWidget::itemClicked, this, &RightStack022::onFontSelected);
    connect(m_cmbSize, &QComboBox::currentIndexChanged, this, &RightStack022::onSizeChanged);
    connect(m_chkBold, &QCheckBox::toggled, this, &RightStack022::onBoldToggled);
    connect(m_chkItalic, &QCheckBox::toggled, this, &RightStack022::onItalicToggled);
    connect(m_cmbFilter, &QComboBox::currentIndexChanged, this, &RightStack022::onFilterChanged);
}