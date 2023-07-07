/**
 * @file helpers/standard-helper.hpp
 */

#ifndef ACCESS_TO_RETRO_STANDARD_HELPER_HPP
#define ACCESS_TO_RETRO_STANDARD_HELPER_HPP

#include <string>
#include <vector>

/// @brief Helper classes and function that help with standard set by access to retro
namespace ar::helpers::standard
{
    /**
     * @brief Gets an icon path for a system if the name provided conforms to access to retro standard or generic one if
     *        it does not
     * @param name Name of the system
     * @return Qt resource path for the icon
     */
    std::string get_console_icon_path_based_on_standard_name(const std::string& name);

    /**
     * @brief Changes console name from Access to Retro standard to a common name
     * @param standard_name Standard console name
     * @return Common console name
     */
    std::string get_console_label_from_standard_name(const std::string& standard_name);

    /**
     * @brief Returns a list of standard console names as defined by Access to Retro standard
     * @return List of standard console names
     */
    std::vector<std::string> get_list_of_standard_console_names();
}

#endif //ACCESS_TO_RETRO_STANDARD_HELPER_HPP
