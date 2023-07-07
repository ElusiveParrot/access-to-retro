#include <sstream>
#include "string-helper.hpp"
#include "cross-platform-helper.hpp"

std::string ar::helpers::cross_platform::get_os_name()
{
#ifdef __linux__
    return "linux";
#elif __APPLE__
    return "darwin";
#elif _WIN32
    return "windows";
#elif __FreeBSD__
    return "bsd";
#elif __unix || __unix__
    return "unix";
#else
    return "unknown";
#endif
}

bool ar::helpers::cross_platform::check_vc_os_compatibility(const std::string& file_name)
{
    std::vector<std::string> file_name_parts = ar::helpers::string::split_string_by_delimiter(file_name, '.');

    if (file_name_parts.size() < 3)
    {
        // Example of vc file: hello.darwin.vc so {hello, darwin, vc} so if less than 3 then invalid
        return false;
    }

    std::size_t last_index = file_name_parts.size() - 1;

    // Extension check
    if (file_name_parts.at(last_index) != "vc")
    {
        return false;
    }

    // OS compatibility check
    std::string compatible_os = file_name_parts.at(last_index - 1);
    std::string host_os       = get_os_name();
    if (compatible_os != host_os)
    {
        return false;
    }

    return true;
}
