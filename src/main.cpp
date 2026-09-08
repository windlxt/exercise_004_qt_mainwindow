#include <iterator> // IWYU pragma: keep
#include <QApplication>
#include "main_window.h"
#include "utility/msg_broker.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    // 注册自定义结构体到Qt元系统
    qRegisterMetaType<BrokerMessage>();
    
    MainWindow w;    
    w.show();
    return a.exec();
}
