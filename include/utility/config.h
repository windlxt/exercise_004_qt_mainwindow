#pragma once

#include <iterator> // IWYU pragma: keep
#include <QWidget>
#include <QFormLayout>
#include <QLineEdit>
#include <QSpinBox>
#include <QCheckBox>
#include <QComboBox>
#include <QPushButton>
#include <QMessageBox>

// UI界面类，数据管理交给 ConfigManager
class ConfigGlobal : public QWidget
{
    Q_OBJECT
public:
    explicit ConfigGlobal(QWidget *parent = nullptr);

private slots:
    void onBtnSaveClicked();
    void onBtnCancelClicked();

private:
    QLineEdit* m_editAppName;
    QSpinBox* m_spinLogLevel;
    QCheckBox* m_chkAutoSave;
    QComboBox* m_cmbTheme;
    QSpinBox* m_spinTimeout;
};