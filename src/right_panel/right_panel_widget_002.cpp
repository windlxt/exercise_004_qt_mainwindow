#include <iterator> // IWYU pragma: keep
#include <qboxlayout.h>
#include <qcombobox.h>
#include <qgridlayout.h>
#include <qlabel.h>
#include <qpushbutton.h>
#include <qstackedwidget.h>
#include <right_panel_widget_002.h>
#include <QLineEdit>

QList<QStringList> cities = {{"广州市", "深圳市", "珠海市", "东莞市"},   // 广东省
                             {"杭州市", "宁波市", "温州市", "绍兴市"},   // 浙江省
                             {"济南市", "青岛市", "潍坊市", "烟台市"},   // 山东省
                             {"南京市", "苏州市", "扬州市", "宿迁市"}};  // 江苏省

RightStack002::RightStack002(QWidget* parent)
    : QWidget(parent)
{
    setupUI();
    connectComponents();
}

void RightStack002::setupUI()
{
    QGridLayout* layout = new QGridLayout();
    QPushButton* m_btn_example = new QPushButton("第二个例子！！");

    m_leName = new QLineEdit("李四");
    m_cboUniversity = new QLineEdit();
    m_cboProvince = new QComboBox();
    m_cboCity = new QComboBox();
    m_leResult = new QLineEdit();    

    // 先设置图标尺寸，放在addItem之前
    m_cboProvince->setIconSize(QSize(24,24));

    QIcon icon(":/resources/icons/about.ico");
    // qDebug() << "图标是否为空？" << icon.isNull();
    // 添加省份
    m_cboProvince->addItem(QIcon(":/resources/icons/about.ico"),"广东省");
    m_cboProvince->addItem(QIcon(":/resources/icons/connect.ico"),"浙江省");
    m_cboProvince->addItem(QIcon(":/resources/icons/email.ico"),"山东省");
    m_cboProvince->addItem(QIcon(":/resources/icons/forum.ico"),"江苏省");

    // // 为省份条目添加图标
    // m_cboProvince->setItemIcon(0, QIcon(":/resources/icons/about.ico"));
    // m_cboProvince->setItemIcon(1, QIcon(":/resources/icons/connect.ico"));
    // m_cboProvince->setItemIcon(2, QIcon(":/resources/icons/email.ico"));
    // m_cboProvince->setItemIcon(3, QIcon(":/resources/icons/forum.ico"));

    // 默认选中第一项-广东省
    m_cboProvince->setCurrentIndex(0);

    // 添加城市
    m_cboCity->addItems(cities[0]);
    
    QLabel* name = new QLabel("姓名 ");
    QLabel* university = new QLabel("大学 ");
    QLabel* province = new QLabel("省份 ");
    QLabel* city = new QLabel("城市 ");
    QLabel* result = new QLabel("简历 ");

    layout->addWidget(m_btn_example,0,0,1,2);
    layout->addWidget(name,1,0,1,1);
    layout->addWidget(m_leName,1,1,1,1);
    layout->addWidget(university,2,0,1,1);
    layout->addWidget(m_cboUniversity,2,1,1,1);
    layout->addWidget(province,3,0,1,1);
    layout->addWidget(m_cboProvince,3,1,1,1);
    layout->addWidget(city,4,0,1,1);
    layout->addWidget(m_cboCity,4,1,1,1);
    layout->addWidget(result,5,0,1,1);
    layout->addWidget(m_leResult,5,1,1,1);
    
    QVBoxLayout* layout_root = new QVBoxLayout();
    layout_root->addLayout(layout);
    layout_root->addStretch(1);
    setLayout(layout_root);      
}

void RightStack002::updateInfo(int index) {
    // 1. 如果是省份组合框，需要先清空城市组合框在添加
    QObject* obj = sender(); // 返回一个指向发送信号对象的指针
    if ( obj ) {
        QComboBox* combo = qobject_cast<QComboBox*>(obj);
        if ( combo == m_cboProvince ) {
            m_cboCity->clear();
            m_cboCity->addItems(cities[index]);
        }
    }

    // 2. 获取姓名、大学、省份、城市
    QString name = m_leName->text();
    QString university = m_cboUniversity->text();
    QString province = m_cboProvince->currentText();
    QString city = m_cboCity->currentText();

    // 3. 显示
    m_leResult->setText(name + ", 毕业于" + university + ", 来自" + province + city);
}

void RightStack002::onLineEditChanged(QString s) {  // 获取姓名、大学、省份、城市
    QString name = m_leName->text();
    QString university = m_cboUniversity->text();
    QString province = m_cboProvince->currentText();
    QString city = m_cboCity->currentText();

    m_leResult->setText(name + ", 毕业于" + university + ", 来自" + province + city);
}

void RightStack002::connectComponents()
{
    // 信号槽
    // connect(m_cboUniversity, &QLineEdit::textChanged, this, &RightStack002::updateInfo);
    connect(m_cboProvince, &QComboBox::currentIndexChanged, this, &RightStack002::updateInfo);
    connect(m_cboCity, &QComboBox::currentIndexChanged, this, &RightStack002::updateInfo);

    connect(m_leName, &QLineEdit::textChanged, this, &RightStack002::onLineEditChanged);
    
}