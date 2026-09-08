#include "utility/msg_broker.h"
#include <QCoreApplication>

MsgBroker& MsgBroker::instance()
{
    // 局部静态，第一次调用时构造，程序退出自动销毁
    static MsgBroker broker;
    // 绑定到主线程，防止线程归属异常
    broker.moveToThread(QCoreApplication::instance()->thread());
    return broker;
}
