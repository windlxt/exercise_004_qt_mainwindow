#include <iterator> // IWYU pragma: keep
#include <QApplication>
#include "main_window.h"
#include "utility/msg_broker.h"

int main(int argc, char *argv[])
{
    //main函数开头设置一次即可
    // qSetMessagePattern("[%{time yyyy-MM-dd hh:mm:ss.zzz}] [%{file}] %{function}(%{line}) %{message}");
    
    QApplication a(argc, argv);
    // 注册自定义结构体到Qt元系统
    qRegisterMetaType<BrokerMessage>();
    
    MainWindow w;    
    w.show();
    return a.exec();
}
