#include <iterator> // IWYU pragma: keep
#include <qboxlayout.h>
#include <qlabel.h>
#include <qpushbutton.h>
#include <qstackedwidget.h>
#include <algorithm/widget_producer_consumer.h>
#include <qtextedit.h>
#include <right_panel_widget_027_display_color.h>
#include <QScrollArea>

#include <QSplitter>
#include <QMouseEvent>
#include <QPalette>

ColorBlock::ColorBlock(const QColor &color, QWidget *parent)
    : QWidget(parent), m_color(color)
{
    setFixedSize(80, 100);
    QVBoxLayout* lay = new QVBoxLayout(this);
    lay->setContentsMargins(4,4,4,4);
    lay->setSpacing(4);

    m_colorLabel = new QLabel();
    m_colorLabel->setFixedHeight(60);
    // 使用QSS设置背景 + 边框
    m_colorLabel->setStyleSheet(QString(R"(
            QLabel{
                background-color:%1;
                border:1px solid #aaaaaa;
            }
            QLabel:hover{
                border:2px solid #3388ff;
            }
        )").arg(m_color.name()));

    m_textLabel = new QLabel(m_color.name().toUpper());
    m_textLabel->setAlignment(Qt::AlignCenter);
    m_textLabel->setStyleSheet("font-size:10px;");

    lay->addWidget(m_colorLabel);
    lay->addWidget(m_textLabel);
}

void ColorBlock::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        emit selectTextColor(m_color);
    }
    else if(event->button() == Qt::RightButton)
    {
        emit selectBgColor(m_color);
    }
    QWidget::mousePressEvent(event);
}


//////////////////////////////////////////////////
RightStack027::RightStack027(QWidget* parent)
    : QWidget(parent)
{
    setupUI();
    connectComponents();    
}

void RightStack027::setupUI()
{
    QSplitter* splitter = new QSplitter(Qt::Horizontal, this);
    splitter->setStretchFactor(0, 1);
    splitter->setStretchFactor(1, 1);
    // ========== 左侧预览区域 ==========
    QWidget* leftWidget = new QWidget();
    QVBoxLayout* leftLay = new QVBoxLayout(leftWidget);
    m_previewLabel = new QLabel("预览文字效果\nHello Qt Color");
    m_previewLabel->setAlignment(Qt::AlignCenter);
    m_previewLabel->setStyleSheet("font-size:24px;");
    leftLay->addWidget(m_previewLabel);
    leftLay->setContentsMargins(20,20,20,20);

    // 默认颜色变量先赋值，但是不要调用 updatePreviewStyle !!!
    m_bgColor = QColor("#FFFFFF");
    m_textColor = QColor("#000000");

    // ========== 右侧颜色面板 ==========
    QWidget* rightWidget = new QWidget();
    QVBoxLayout* rightVLayout = new QVBoxLayout(rightWidget);
    rightVLayout->setContentsMargins(10,10,10,10);
    rightVLayout->setSpacing(12);

    // ---------- 新增：右侧顶部选中颜色预览行 ----------
    QWidget* selectedPreviewWidget = new QWidget();
    QHBoxLayout* selectedHLayout = new QHBoxLayout(selectedPreviewWidget);
    selectedHLayout->setSpacing(16);

    // ✅ 在这里new所有预览label
    m_textColorPreviewLabel = new QLabel();
    m_textColorPreviewLabel->setFixedSize(60,40);
    m_textColorValueLabel = new QLabel("文字色: #000000");
    
    m_bgColorPreviewLabel = new QLabel();
    m_bgColorPreviewLabel->setFixedSize(60,40);    
    m_bgColorValueLabel = new QLabel("背景色: #FFFFFF");

    selectedHLayout->addWidget(m_textColorPreviewLabel);
    selectedHLayout->addWidget(m_textColorValueLabel);
    selectedHLayout->addWidget(m_bgColorPreviewLabel);
    selectedHLayout->addWidget(m_bgColorValueLabel);
    selectedHLayout->addStretch();

    rightVLayout->addWidget(selectedPreviewWidget);

    // ---------- 原来的颜色网格 ----------
    QGridLayout* gridLay = new QGridLayout();
    gridLay->setSpacing(8);
    rightVLayout->addLayout(gridLay);

    // 美工常用色表，可以继续扩充
    QList<QColor> colorList = {
        // 基础黑白 + 灰度(20个)
        QColor("#FFFFFF"), QColor("#F8F9FA"), QColor("#E9ECEF"), QColor("#DEE2E6"),
        QColor("#CED4DA"), QColor("#ADB5BD"), QColor("#868E96"), QColor("#6C757D"),
        QColor("#495057"), QColor("#343A40"), QColor("#212529"), QColor("#000000"),
        QColor("#F5F5F5"), QColor("#EEEEEE"), QColor("#E0E0E0"), QColor("#BDBDBD"),
        QColor("#9E9E9E"), QColor("#757575"), QColor("#616161"), QColor("#424242"),
        // 红色系(10)
        QColor("#FFEBEE"), QColor("#FFCDD2"), QColor("#EF9A9A"), QColor("#E57373"),
        QColor("#EF5350"), QColor("#F44336"), QColor("#E53935"), QColor("#D32F2F"),
        QColor("#C62828"), QColor("#B71C1C"),
        // 橙色系(10)
        QColor("#FFF3E0"), QColor("#FFE0B2"), QColor("#FFCC80"), QColor("#FFB74D"),
        QColor("#FFA726"), QColor("#FF9800"), QColor("#FB8C00"), QColor("#F57C00"),
        QColor("#EF6C00"), QColor("#E65100"),
        // 黄色系(10)
        QColor("#FFFDE7"), QColor("#FFF9C4"), QColor("#FFF176"), QColor("#FFEE58"),
        QColor("#FFEB3B"), QColor("#FDD835"), QColor("#FBC02D"), QColor("#F9A825"),
        QColor("#F57F17"), QColor("#F1C40F"),
        // 绿色系(10)
        QColor("#E8F5E9"), QColor("#C8E6C9"), QColor("#A5D6A7"), QColor("#81C784"),
        QColor("#66BB6A"), QColor("#4CAF50"), QColor("#43A047"), QColor("#388E3C"),
        QColor("#2E7D32"), QColor("#1B5E20"),
        // 青色/蓝绿系(10)
        QColor("#E0F2F1"), QColor("#B2DFDB"), QColor("#80CBC4"), QColor("#4DB6AC"),
        QColor("#26A69A"), QColor("#009688"), QColor("#00897B"), QColor("#00796B"),
        QColor("#00695C"), QColor("#004D40"),
        // 蓝色系(10)
        QColor("#E3F2FD"), QColor("#BBDEFB"), QColor("#90CAF9"), QColor("#64B5F6"),
        QColor("#42A5F5"), QColor("#2196F3"), QColor("#1E88E5"), QColor("#1976D2"),
        QColor("#1565C0"), QColor("#0D47A1"),
        // 紫色系(10)
        QColor("#F3E5F5"), QColor("#E1BEE7"), QColor("#CE93D8"), QColor("#BA68C8"),
        QColor("#AB47BC"), QColor("#9C27B0"), QColor("#8E24AA"), QColor("#7B1FA2"),
        QColor("#6A1B9A"), QColor("#4A148C"),
        // 马卡龙柔和附加色(10)
        QColor("#FFC0CB"), QColor("#FFA07A"), QColor("#98FB98"), QColor("#87CEFA"),
        QColor("#DDA0DD"), QColor("#FFD700"), QColor("#CD853F"), QColor("#20B2AA"),
        QColor("#FF6347"), QColor("#9370DB")
    };


    int colCnt = 5;
    int row = 0, col = 0;
    for(const auto& c : colorList)
    {
        ColorBlock* block = new ColorBlock(c);
        connect(block, &ColorBlock::selectTextColor, this, &RightStack027::onSelectTextColor);
        connect(block, &ColorBlock::selectBgColor, this, &RightStack027::onSelectBgColor);
        gridLay->addWidget(block, row, col);
        col++;
        if(col >= colCnt)
        {
            col = 0;
            row++;
        }
    }

    // ========== 滚动区域 ==========
    QScrollArea* scrollArea = new QScrollArea;
    scrollArea->setWidget(rightWidget);
    scrollArea->setWidgetResizable(true);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    splitter->addWidget(leftWidget);
    splitter->addWidget(scrollArea);

    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->addWidget(splitter);

    // ✅ 全部控件构造完成之后，再初始化渲染！！！
    updatePreviewStyle();
}

void RightStack027::updateRightColorPreview()
{
    // 空指针判断，防止控件未创建就访问
    if (!m_textColorPreviewLabel || !m_bgColorPreviewLabel || !m_textColorValueLabel || !m_bgColorValueLabel)
        return;

    // 更新文字色预览块
    QPalette palText = m_textColorPreviewLabel->palette();
    m_textColorPreviewLabel->setStyleSheet(QString("background-color: %1; border:1px solid #999;").arg(m_textColor.name()));
    m_textColorValueLabel->setText(QString("文字色: %1").arg(m_textColor.name().toUpper()));

    m_bgColorPreviewLabel->setStyleSheet(QString("background-color: %1; border:1px solid #999;").arg(m_bgColor.name()));
    m_bgColorValueLabel->setText(QString("背景色: %1").arg(m_bgColor.name().toUpper()));

    // 更新背景色预览块
    QPalette palBg = m_bgColorPreviewLabel->palette();
    palBg.setColor(QPalette::Window, m_bgColor);
    m_bgColorPreviewLabel->setAutoFillBackground(true);
    m_bgColorPreviewLabel->setPalette(palBg);
    m_bgColorValueLabel->setText(QString("背景色: %1").arg(m_bgColor.name().toUpper()));
}


void RightStack027::updatePreviewStyle()
{
    if (!m_previewLabel)
        return;

    QString css = QString("background-color:%1; color:%2; font-size:24px;")
                      .arg(m_bgColor.name())
                      .arg(m_textColor.name());
    m_previewLabel->setStyleSheet(css);

    updateRightColorPreview();
}


void RightStack027::onSelectTextColor(const QColor& c)
{
    m_textColor = c;
    updatePreviewStyle();
}
void RightStack027::onSelectBgColor(const QColor& c)
{
    m_bgColor = c;
    updatePreviewStyle();
}

void RightStack027::connectComponents()
{

}