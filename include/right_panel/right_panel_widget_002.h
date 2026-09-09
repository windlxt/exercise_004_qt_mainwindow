#pragma once

#include <iterator> // IWYU pragma: keep
#include <qabstractspinbox.h>
#include <qcombobox.h>
#include <qstackedwidget.h>
#include <qtextedit.h>

class QComboBox;
class QLineEdit;

class RightStack002: public QWidget
{
    Q_OBJECT
public:
    RightStack002(QWidget* parent=nullptr);
    
private:
    void setupUI();
    void connectComponents();

    QLineEdit* m_leName;
    QComboBox* m_cboProvince;
    QLineEdit* m_cboUniversity;
    QComboBox* m_cboCity;
    QLineEdit* m_leResult;
    
    
private slots:
    void updateInfo(int index);
    void onLineEditChanged(QString s);

};