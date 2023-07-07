/**
 * @file helpers/cross-platform-helper.hpp
 */

#ifndef ACCESS_TO_RETRO_CROSS_PLATFORM_HELPER_HPP
#define ACCESS_TO_RETRO_CROSS_PLATFORM_HELPER_HPP

#include <string>
#include <vector>

/// @brief Helper classes and function that help with ensuring portability across various platforms
namespace ar::helpers::cross_platform
{
    /**
     * @brief Returns a string based on operating system detected at run-time.
     * @warning Due to performance concerns this should only be used for logging or any other user facing output, for
     *          compile-time guarding of code based on OS macros should be used instead as they do not have any run-time
     *          overhead.
     * @return String with OS name
     */
    std::string get_os_name();

    /**
     * @brief Validates that the virtual console file was compiled for user's platform
     * @param file_name The part of the virtual console path that contains file name only
     * @return Whether it is compatible or not
     */
    bool check_vc_os_compatibility(const std::string& file_name);
}

#endif //ACCESS_TO_RETRO_CROSS_PLATFORM_HELPER_HPP
