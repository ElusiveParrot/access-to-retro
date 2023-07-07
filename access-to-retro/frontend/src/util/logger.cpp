#include <iostream>
#include "util/settings-manager.hpp"
#include "util/time.hpp"
#include "logger.hpp"

static ar::util::logger g_logger;

static std::string get_time_string()
{
    ar::util::time_and_date now = ar::util::time_and_date::now();

    return now.as_string("h:m:s");
}

static ar::util::logger::priority get_priority_from_string(const std::string& str)
{
    // NOTE TO MARKER: You cannot use switch statement with a string in C++
    if (str == "-EVERYTHING_DEV")
    {
        return ar::util::logger::priority::EVERYTHING_DEV;
    }
    else if (str == "-EVERYTHING")
    {
        return ar::util::logger::priority::EVERYTHING;
    }
    else if (str == "-WARNING")
    {
        return ar::util::logger::priority::WARNING;
    }
    else if (str == "-APP_ERROR")
    {
        return ar::util::logger::priority::APP_ERROR;
    }
    else if (str == "-CRITICAL")
    {
        return ar::util::logger::priority::CRITICAL;
    }
    else if (str == "-NOTHING")
    {
        return ar::util::logger::priority::NOTHING;
    }
    else
    {
        LOG_ERROR("Invalid priority argument provided: " + str + ", defaulting to CRITICAL");

        return ar::util::logger::priority::CRITICAL;
    }
}

static ar::util::logger::verbosity get_verbosity_from_string(const std::string& str)
{
    if (str == "-DEBUG")
    {
        return ar::util::logger::verbosity::DEBUG;
    }
    else if (str == "-RICH")
    {
        return ar::util::logger::verbosity::RICH;
    }
    else if (str == "-SIMPLE")
    {
        return ar::util::logger::verbosity::SIMPLE;
    }
    else
    {
        LOG_ERROR("Invalid verbosity argument provided: " + str + ", defaulting to SIMPLE");

        return ar::util::logger::verbosity::SIMPLE;
    }
}

void ar::util::logger::configure_based_on_args(const std::vector<std::string>& args)
{
    if (args.empty())
    {
        ar::util::logger::set_minimum_priority(priority::CRITICAL);
        ar::util::logger::set_verbosity(verbosity::SIMPLE);

        LOG_INFO("No arguments for the logger, assuming defaults (CRITICAL, SIMPLE)");

        return;
    }

    const std::string& priority_string = args.at(0);
    priority new_priority = get_priority_from_string(priority_string);

    /*
     * Defaults in case they are not provided
     *
     */
    std::string verbosity_string = "SIMPLE";
    verbosity   new_verbosity    = verbosity::SIMPLE;

    // Provided both priority and verbosity
    if (args.size() > 1)
    {
        verbosity_string = args.at(1);
        new_verbosity    = get_verbosity_from_string(verbosity_string);
    }

    ar::util::logger::set_minimum_priority(new_priority);
    ar::util::logger::set_verbosity(new_verbosity);

    LOG_INFO("Setting minimum priority to: " + priority_string + " and verbosity to: " + verbosity_string);
}

void ar::util::logger::configure_based_on_settings()
{
    std::shared_ptr<ar::util::settings_manager> settings_manager = ar::util::settings_manager::get_global_manager();
    if (settings_manager == nullptr)
    {
        throw std::runtime_error("Attempting to configure logger using app settings when settings_manager=nullptr");
    }

    auto logger_priority_setting_res = settings_manager->get_setting("logger_priority");
    if (logger_priority_setting_res != std::nullopt)
    {
        std::string value = logger_priority_setting_res.value();
        g_logger._minimum_priority = static_cast<ar::util::logger::priority>(std::stoi(value));
    }

    auto logger_verbosity_setting_res = settings_manager->get_setting("logger_verbosity");
    if (logger_verbosity_setting_res != std::nullopt)
    {
        std::string value = logger_verbosity_setting_res.value();
        g_logger._current_verbosity = static_cast<ar::util::logger::verbosity>(std::stoi(value));
    }
}

void ar::util::logger::log(const std::string& file, const std::string& function, int line,
                           util::logger::priority priority, const std::string& module, const std::string& prefix,
                           const std::string& message)
{
    // Do not log anything below minimum priority
    if (static_cast<int>(priority) < static_cast<int>(g_logger._minimum_priority))
    {
        return;
    }

    switch (g_logger._current_verbosity)
    {
        // Message only
        case util::logger::verbosity::SIMPLE:
            std::cout << message << std::endl;
            break;

            // Message + time (coloured)
        case util::logger::verbosity::RICH:
            std::cout << "[" + get_time_string() + "][" + prefix + "] " << message << std::endl;
            break;

            // Message + time + debug info
        case util::logger::verbosity::DEBUG:
            std::cout << "[" + get_time_string() + "][" << module << "][" << function << "(..)][" + prefix + "] " <<
                      message << ". Originated in: " << file << " at line: " << std::to_string(line) << std::endl;
            break;
    }
}

void ar::util::logger::log(const std::string& file, const std::string& function, int line,
                           ar::util::logger::priority priority, const std::string& module, const ar::base_exception& ex)
{
    ar::util::logger::log(file, function, line, priority, module, "EXCEPTION", ex.get_logger_formatted_error());
}

void ar::util::logger::set_verbosity(util::logger::verbosity new_verbosity)
{
    if (new_verbosity == util::logger::verbosity::DEBUG)
    {
        g_logger._minimum_priority = util::logger::priority::EVERYTHING_DEV;
        LOG_INFO("Minimum priority changed to index: 'EVERYTHING_DEV' due to new verbosity setting ");
    }

    g_logger._current_verbosity = new_verbosity;

    LOG_INFO("Verbosity changed to index: '" + std::to_string((int) new_verbosity) + "'");
}

void ar::util::logger::set_minimum_priority(util::logger::priority new_minimum_priority)
{
    if (new_minimum_priority == util::logger::priority::EVERYTHING_DEV)
    {
        g_logger._current_verbosity = util::logger::verbosity::DEBUG;
        LOG_INFO("Verbosity changed to index: 'DEBUG' due to new priority setting ");
    }
    else if (new_minimum_priority == util::logger::priority::NOTHING)
    {
        g_logger._current_verbosity = util::logger::verbosity::RICH;
        LOG_INFO("Verbosity changed to index: 'RICH' due to new priority setting ");
    }

    g_logger._minimum_priority = new_minimum_priority;

    LOG_INFO("Minimum priority changed to index: '" + std::to_string((int) new_minimum_priority) + "'");
}
