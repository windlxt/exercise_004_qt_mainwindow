#include <utility/config_manager.h>
#include <QCoreApplication>

ConfigManager* ConfigManager::m_instance = nullptr;

ConfigManager* ConfigManager::getInstance()
{
    // C++11及以上，局部静态是线程安全的Meyers单例，推荐
    static ConfigManager ins;
    return &ins;
}

ConfigManager::ConfigManager(QObject *parent)
    : QObject(parent)
{
    m_settings = new QSettings("MyCompany", "MyApp", this);
    loadConfig();
}

GlobalConfigData ConfigManager::getConfig() const
{
    return m_cfg;
}

void ConfigManager::loadConfig()
{
    m_cfg.appName      = m_settings->value("Config/appName", "MyApp").toString();
    m_cfg.logLevel     = m_settings->value("Config/logLevel", 2).toInt();
    m_cfg.autoSave     = m_settings->value("Config/autoSave", true).toBool();
    m_cfg.themeMode    = m_settings->value("Config/themeMode", "light").toString();
    m_cfg.timeoutSec   = m_settings->value("Config/timeoutSec",30).toInt();
}

void ConfigManager::saveConfig(const GlobalConfigData &cfg)
{
    m_cfg = cfg;

    m_settings->setValue("Config/appName", m_cfg.appName);
    m_settings->setValue("Config/logLevel", m_cfg.logLevel);
    m_settings->setValue("Config/autoSave", m_cfg.autoSave);
    m_settings->setValue("Config/themeMode", m_cfg.themeMode);
    m_settings->setValue("Config/timeoutSec", m_cfg.timeoutSec);
    m_settings->sync();

    emit sigConfigChanged(m_cfg);
}
