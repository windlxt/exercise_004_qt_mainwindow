#pragma once

#include <iterator> // IWYU pragma: keep
#include <QObject>
#include <QString>
#include <qvariant.h>

// 消息结构体（按需扩展字段）
struct BrokerMessage
{
    QString src;        // 发送源标识
    QString cmd;        // 命令字
    QVariant payload;   // 载荷，可以传任意类型
};

Q_DECLARE_METATYPE(BrokerMessage)

class MsgBroker : public QObject
{
    Q_OBJECT
public:
    // 获取单例实例
    static MsgBroker& instance();

    // 删除拷贝/移动，禁止复制
    MsgBroker(const MsgBroker&) = delete;
    MsgBroker& operator=(const MsgBroker&) = delete;
    MsgBroker(MsgBroker&&) = delete;
    MsgBroker& operator=(MsgBroker&&) = delete;

signals:
    // 全局消息广播信号，所有订阅者接收
    void onMessage(const BrokerMessage& msg);

private:
    // 私有构造
    MsgBroker() = default;
};
