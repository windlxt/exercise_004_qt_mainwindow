#include <iterator> // IWYU pragma: keep
#include <qboxlayout.h>
#include <qcontainerfwd.h>
#include <qframe.h>
#include <qlineedit.h>
#include <qnamespace.h>
#include <qpushbutton.h>
#include <qstackedwidget.h>
#include <qtextedit.h>
#include <right_panel_widget_025_concurrent_futurewatcher.h>
#include <QLabel>
#include <QLineEdit>
#include <QFileDialog>
#include <QFile>
#include <QCoreApplication>
#include <utility/global_const_function.h>
#include <chrono>
#include <thread>
#include <QScrollArea>


RightStack025::RightStack025(QWidget* parent)
    : QWidget(parent)
{
    setupUI();
    connectComponents();
}

RightStack025::~RightStack025()
{
    
}

void RightStack025::setupUI()
{       
    QVBoxLayout* layout = new QVBoxLayout();      
}

// 信号槽连接
void RightStack025::connectComponents()
{
    
}
