/**
 * @file util/logger.hpp
 */

#ifndef ACCESS_TO_RETRO_FRONTEND_LOGGER_HPP
#define ACCESS_TO_RETRO_FRONTEND_LOGGER_HPP

#include <QVariant>
#include <vector>
#include <string>
#include "error/base-exception.hpp"

/// @brief Shortcut macros for logging
#define LOG(priority, ar_module, prefix, to_log) ar::util::logger::log(__FILE__, __func__, __LINE__, priority, \
ar_module, prefix, to_log)
#define LOG_EXCEPTION(ar_module, ex) ar::util::logger::log(__FILE__, __func__, __LINE__, ar::util::logger::priority::ERROR, \
ar_module, ex)

/// @brief Shortcut macros for non-exception logging with pre-defined priority, prefix and/or module
#define LOG_DEBUG(ar_module, message)  LOG(ar::util::logger::priority::EVERYTHING_DEV, ar_module, "DEBUG",     message)
#define LOG_INFO(message)              LOG(ar::util::logger::priority::EVERYTHING,     "",     "INFO",      message)
#define LOG_WARNING(message)           LOG(ar::util::logger::priority::WARNING,        "",     "WARNING ",  message)
#define LOG_ERROR(message)             LOG(ar::util::logger::priority::APP_ERROR,          "",     "ERROR",     message)
#define LOG_CRITICAL(message)          LOG(ar::util::logger::priority::CRITICAL,       "",     "CRITICAL",  message)

/// @brief Contains classes with various functionality that are not part of 'core'
namespace ar::util
{
    /// @brief Used to display debug messages for the developer or/and to the user.
    class logger
    {
    public:
        /// @brief Represents verbosity of the message and decides on formatting of the message.
        enum class verbosity
        {
            /// @brief Message ONLY.
            SIMPLE,

            /// @brief Message and time.
            RICH,

            /// @brief Message, time, module, file, function and line, forces EVERYTHING_DEV priority.
            DEBUG,
        };

        /**
         * @brief Represents priority of the message and decides whether it is shown or not based on the current logger
         *        settings.
         * @details Minimum priority can be changed using set_minimum_priority().
         */
        enum class priority
        {
            /// @brief No filter, forces DEBUG verbosity.
            EVERYTHING_DEV,

            /// @brief Only filters debug messages.
            EVERYTHING,

            /// @brief Only shows warnings and all error.
            WARNING,

            /// @brief Only shows errors.
            APP_ERROR,

            /// @brief Only shows critical errors.
            CRITICAL,

            /// @brief Logger disabled, forces RICH verbosity (to avoid confusion)
            NOTHING,
        };

        /**
         * @brief Configure global logger based on command line arguments.
         * @details First argument decides verbosity (for ex: "-SIMPLE") while second priority (for ex:
         *          "EVERYTHING_DEV"), argument name is 1:1 mapped to enum entry.
         * @param args Vector with arguments that should be constructed from argv and argc
         */
        static void configure_based_on_args(const std::vector<std::string>& args);

        /**
         * @brief Configure global logger based on app settings.
         * @warning If settings manager wasn't configured using 'configure' method then this method will fail
         * @throws Exceptions:
         *  - std::runtime_error: Settings manager wasn't configured
         */
        static void configure_based_on_settings();

        /**
         * @brief Direct method to log message to stdout.
         * @details Should be avoid and macros LOG_x should be used instead. Logger should be used as soon as
         *          possible when the error happens so that parameters can correspond to the area where error happened.
         * @param file Source file where the logger is being used.
         * @param function Function in which the logger is being used.
         * @param line Line in the source code in which the logger is being used.
         * @param priority Priority of the error.
         * @param module Module of the program in which the error happened. Only displayed in debug messages,
         * @param prefix Prefix that will be shown before the message
         * @param message The message that should be logged.
         */
        static void log(const std::string& file, const std::string& function, int line,
                        util::logger::priority priority, const std::string& module, const std::string& prefix,
                        const std::string& message);

        /**
         * @brief Direct method to log message to stdout.
         * @details Should be avoid and macros LOG_EXCEPTION should be used instead. Logger should be used as soon as
         *          possible when the error happens so that parameters can correspond to the area where error happened.
         * @param file Source file where the logger is being used.
         * @param function Function in which the logger is being used.
         * @param line Line in the source code in which the logger is being used.
         * @param priority Priority of the error.
         * @param module Module of the program in which the error happened. Only displayed in debug messages,
         * @param ex Exception that should be logger.
         */
        static void log(const std::string& file, const std::string& function, int line,
                        util::logger::priority priority, const std::string& module, const ar::base_exception& ex);

        static void set_verbosity(util::logger::verbosity new_verbosity);
        static void set_minimum_priority(util::logger::priority new_minimum_priority);

    private:
        /// @brief Amount of details shown with the message
        util::logger::verbosity _current_verbosity = util::logger::verbosity::SIMPLE;

        /// @brief If message verbosity is below it then it will not show
        util::logger::priority _minimum_priority = util::logger::priority::CRITICAL;
    };
} // util

// Declare some classes as Qt's meta types
Q_DECLARE_METATYPE(ar::util::logger::priority);

Q_DECLARE_METATYPE(ar::util::logger::verbosity);

#endif //ACCESS_TO_RETRO_FRONTEND_LOGGER_HPP
