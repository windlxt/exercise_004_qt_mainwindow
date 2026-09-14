#include <utility/config.h>
#include <utility/config_manager.h>
#include <QVBoxLayout>
#include <QLabel>
#include <QGroupBox>
#include <QMessageBox>
#include <QPushButton>
#include <QLineEdit>
#include <QSpinBox>
#include <QCheckBox>
#include <QComboBox>

ConfigGlobal::ConfigGlobal(QWidget *parent)
    : QWidget(parent)
{
    setWindowTitle("全局配置");
    setMinimumSize(460, 360);

    // ========= 主布局 =========
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(20,20,20,20);
    mainLayout->setSpacing(16);

    // ===================== 第一组：基础设置 QGroupBox =====================
    QGroupBox* groupBase = new QGroupBox("基础设置");
    QFormLayout* formBase = new QFormLayout(groupBase);
    formBase->setHorizontalSpacing(12);
    formBase->setVerticalSpacing(10);
    formBase->setContentsMargins(16,16,16,16);
    formBase->setLabelAlignment(Qt::AlignRight); // 标签统一右对齐

    m_editAppName = new QLineEdit();
    formBase->addRow("应用名称：", m_editAppName);

    m_spinLogLevel = new QSpinBox();
    m_spinLogLevel->setRange(0,5);
    formBase->addRow("日志等级(0~5)：", m_spinLogLevel);

    m_chkAutoSave = new QCheckBox("开启自动保存");
    formBase->addRow("自动保存：", m_chkAutoSave);

    // ===================== 第二组：界面设置 QGroupBox =====================
    QGroupBox* groupUi = new QGroupBox("界面设置");
    QFormLayout* formUi = new QFormLayout(groupUi);
    formUi->setHorizontalSpacing(12);
    formUi->setVerticalSpacing(10);
    formUi->setContentsMargins(16,16,16,16);
    formUi->setLabelAlignment(Qt::AlignRight);

    m_cmbTheme = new QComboBox();
    m_cmbTheme->addItems({"light", "dark"});
    formUi->addRow("界面主题：", m_cmbTheme);

    m_spinTimeout = new QSpinBox();
    m_spinTimeout->setRange(1,300);
    formUi->addRow("超时秒数：", m_spinTimeout);

    // ===================== 底部按钮区域 =====================
    QHBoxLayout* btnLayout = new QHBoxLayout();
    QPushButton* btnSave = new QPushButton("保存");
    QPushButton* btnCancel = new QPushButton("取消");
    btnLayout->addStretch();
    btnLayout->addWidget(btnSave);
    btnLayout->addWidget(btnCancel);

    // 全部加入主布局
    mainLayout->addWidget(groupBase);
    mainLayout->addWidget(groupUi);
    mainLayout->addStretch(); // 占位，把按钮压到底部
    mainLayout->addLayout(btnLayout);

    connect(btnSave, &QPushButton::clicked, this, &ConfigGlobal::onBtnSaveClicked);
    connect(btnCancel, &QPushButton::clicked, this, &ConfigGlobal::onBtnCancelClicked);

    // 加载当前配置填充UI
    auto cfg = ConfigManager::getInstance()->getConfig();
    m_editAppName->setText(cfg.appName);
    m_spinLogLevel->setValue(cfg.logLevel);
    m_chkAutoSave->setChecked(cfg.autoSave);
    m_cmbTheme->setCurrentText(cfg.themeMode);
    m_spinTimeout->setValue(cfg.timeoutSec);
}

void ConfigGlobal::onBtnSaveClicked()
{
    GlobalConfigData newCfg;
    newCfg.appName    = m_editAppName->text();
    newCfg.logLevel   = m_spinLogLevel->value();
    newCfg.autoSave   = m_chkAutoSave->isChecked();
    newCfg.themeMode  = m_cmbTheme->currentText();
    newCfg.timeoutSec = m_spinTimeout->value();
    ConfigManager::getInstance()->saveConfig(newCfg);
    // QMessageBox::information(this,"提示","配置已保存！");
    this->close();
}

void ConfigGlobal::onBtnCancelClicked()
{
    this->close();
}
