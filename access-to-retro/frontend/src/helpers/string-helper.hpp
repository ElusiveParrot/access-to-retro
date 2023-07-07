/**
 * @file helpers/string-helper.hpp
 */

#ifndef ACCESS_TO_RETRO_STRING_HELPER_HPP
#define ACCESS_TO_RETRO_STRING_HELPER_HPP

#include <vector>
#include <string>

/// @brief Helper classes and function that help with string manipulation
namespace ar::helpers::string
{
    /**
     * @brief Split string into tokens based on delimiter
     * @param str String to be split
     * @param delimiter Delimiter character
     * @return Tokenized string in a vector
     */
    std::vector<std::string> split_string_by_delimiter(const std::string& str, char delimiter);
}

#endif //ACCESS_TO_RETRO_STRING_HELPER_HPP
