#include <sstream>
#include "string-helper.hpp"

std::vector<std::string> ar::helpers::string::split_string_by_delimiter(const std::string& str, char delimiter)
{
    std::vector<std::string> parts;
    std::stringstream        stream(str);

    std::string tmp;

    while (std::getline(stream, tmp, delimiter))
    {
        parts.push_back(tmp);
    }

    return parts;
}
