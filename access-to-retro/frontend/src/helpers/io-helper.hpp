#ifndef ACCESS_TO_RETRO_IO_HELPER_HPP
#define ACCESS_TO_RETRO_IO_HELPER_HPP

#include <string>

/// @brief Helper classes and function that help with IO operations
namespace ar::helpers::io
{
    /**
     * @brief Checks whether a file extension is contained within a string containing extensions
     * @param extension File extension
     * @param extensions_string String containing extensions for file picker (ex: *.jpg)
     * @return Whether file matches the extension query
     */
    bool is_extension_part_of_extensions_string(const std::string& extension, const std::string& extensions_string);

    /**
     * @brief Copy file from provided file path to provided directory
     * @param file_path File to be copied
     * @param output_directory Directory to which the file will be copied
     * @throws Exceptions:
     *  - ar::error:io_error: Unable to copy the file
     */
    void copy_file(const std::string& file_path, const std::string& output_directory);
}

#endif //ACCESS_TO_RETRO_IO_HELPER_HPP
