#ifndef ACCESS_TO_RETRO_SETTINGS_MANAGER_HPP
#define ACCESS_TO_RETRO_SETTINGS_MANAGER_HPP

#include <QSettings>
#include <QDir>
#include <optional>

namespace ar::util
{
    /// @brief Class used to manage application settings, wrapped for QSettings
    class settings_manager
    {
    public:
        /**
         * @brief Default constructor, should NOT be used
         * @throws Exceptions:
         *  - ar::error::io_error: Unable to create new settings file if it wasn't found
         * @details Should not be used anywhere outside of implementation file to create a singleton,, use
         *          'get_global_manager' instead to access the singleton
         */
        settings_manager();

        /**
         * @brief Configures the setting manager singleton
         * @throws Exceptions:
         *  - ar::error::io_error: Unable to create new settings file if it wasn't found
         * @details Should be called somewhere at the beginning of the program, before 'get_global_manager' is used.
         */
        static void configure();

        /**
         * @brief Getter for a singleton settings manager object
         * @remark Useful when you want to store pointer to settings manager as a class member
         * @warning If 'configure' wasn't called it wil return a null pointer
         * @return Global singleton of settings manager or a null pointer if 'configure' wasn't used before this call
         */
        static std::shared_ptr<ar::util::settings_manager> get_global_manager();

        /**
         * @brief Getter for a path of Access to Retro settings file, the file might not be there if the program
         *        didn't run before and 'configure' was not yet used.
         * @return Path to the configuration file
         */
        [[nodiscard]] static std::string get_settings_file_path();

        /**
         * @brief Getter for settings inside the settings file.
         * @param key Name of the setting
         * @return String value for that setting or nothing if it does not exist
         */
        [[nodiscard]] std::optional<std::string> get_setting(const std::string& key);

        /**
         * @brief Getter for settings inside the settings file, if the value doesn't exist the it gets set.
         * @param key Name of the setting
         * @param default_value Value that the setting will be set to if it does not exist
         * @return String value for that setting
         */
        [[nodiscard]] std::string get_setting_or_set_if_not_exists(const std::string& key,
                                                                   const std::string& default_value);

        /**
         * @brief Setter for settings inside the settings file
         * @param key Name of the setting
         * @param value New/initial value
         */
        void set_setting(const std::string& key, const std::string& value);

    private:
        /// @brief Whether the setting manager has been configured
        bool _configured = false;

        /// @brief Internal QSettings object
        QSettings _settings;

        /**
         * @brief Helper function to create QSettings object
         * @throws Exceptions:
         *  - ar::error::io_error: Unable to create new settings file if it wasn't found
         * @return Configured QSettings object
         */
        static QSettings create_qsettings_object_and_or_file();
    };
}


#endif //ACCESS_TO_RETRO_SETTINGS_MANAGER_HPP
