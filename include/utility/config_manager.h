#pragma once

#include <iterator> // IWYU pragma: keep
#include <QObject>
#include <QSettings>

struct GlobalConfigData
{
    QString appName;
    int logLevel;
    bool autoSave;
    QString themeMode;
    int timeoutSec;
};

// 数据管理类，不能有UI控件
class ConfigManager : public QObject
{
    Q_OBJECT
public:
    static ConfigManager* getInstance();
    // ~ConfigManager() override; // 声明析构函数
    ConfigManager(const ConfigManager&)=delete;
    ConfigManager& operator=(const ConfigManager&)=delete;

    GlobalConfigData getConfig() const;
    void loadConfig();
    void saveConfig(const GlobalConfigData& cfg);

signals:
    void sigConfigChanged(const GlobalConfigData& newCfg);

private:
    explicit ConfigManager(QObject* parent=nullptr);
    static ConfigManager* m_instance;
    GlobalConfigData m_cfg;
    QSettings* m_settings;
};