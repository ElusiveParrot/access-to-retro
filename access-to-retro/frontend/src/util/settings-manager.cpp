#include <QFile>
#include <memory>
#include "error/io-error.hpp"
#include "util/logger.hpp"
#include "settings-manager.hpp"

// Singleton for global use
static std::shared_ptr<ar::util::settings_manager> g_settings_manager = nullptr;

ar::util::settings_manager::settings_manager() :
        _settings(create_qsettings_object_and_or_file())
{
    if (g_settings_manager != nullptr)
    {
        throw std::runtime_error("Broken singleton pattern of settings manager");
    }
}

void ar::util::settings_manager::configure()
{
    g_settings_manager = std::make_shared<ar::util::settings_manager>();
    g_settings_manager->_configured = true;

    LOG_INFO("Settings manager configured successfully..");
}

std::shared_ptr<ar::util::settings_manager> ar::util::settings_manager::get_global_manager()
{
    return g_settings_manager;
}

std::string ar::util::settings_manager::get_settings_file_path()
{
    QDir settings_file_path = QDir::homePath() + QDir::separator() + "access-to-retro-settings.ini";

    return settings_file_path.absolutePath().toStdString();
}

QSettings ar::util::settings_manager::create_qsettings_object_and_or_file()
{
    QString settings_file_path = QString::fromStdString(get_settings_file_path());

    // Settings file doesn't exist
    if (!QFile(settings_file_path).exists())
    {
        LOG_INFO("Settings file not found, creating new at: '" + settings_file_path.toStdString() + "'");

        QFile file(settings_file_path);

        // Open = create if not exists
        if (!file.open(QIODevice::WriteOnly))
        {
            throw ar::error::io_error(settings_file_path.toStdString(), "Unable to create settings file");
        }

        file.close();
    }

    return { settings_file_path, QSettings::IniFormat };
}

std::optional<std::string> ar::util::settings_manager::get_setting(const std::string& key)
{
    if (_settings.contains(QString::fromStdString(key)))
    {
        return _settings.value(QString::fromStdString(key)).toString().toStdString();
    }
    else
    {
        return std::nullopt;
    }
}

std::string ar::util::settings_manager::get_setting_or_set_if_not_exists(const std::string& key,
                                                                         const std::string& default_value)
{
    std::optional<std::string> current_value = get_setting(key);
    if (current_value == std::nullopt)
    {
        // Setting doesn't exist so set it...
        set_setting(key, default_value);

        // ...and return the default value back
        return default_value;
    }
    else
    {
        // Setting exists so return it
        return current_value.value();
    }
}


void ar::util::settings_manager::set_setting(const std::string& key, const std::string& value)
{
    _settings.setValue(QString::fromStdString(key), QString::fromStdString(value));

    LOG_DEBUG("util.settings_manager", "Changed setting '" + key + "' value to '" + value + "'");
}
