#include <filesystem>
#include "error/io-error.hpp"
#include "io-helper.hpp"

bool ar::helpers::io::is_extension_part_of_extensions_string(const std::string& extension,
                                                             const std::string& extensions_string)
{
    // TODO: Use Qt to implement it? this is bug prone
    return extensions_string.find(extension) != std::string::npos;
}

void ar::helpers::io::copy_file(const std::string& file_path, const std::string& output_directory)
{
    std::string           file_name   = std::filesystem::path(file_path).filename().string();
    std::filesystem::path output_path = std::filesystem::path(output_directory) / std::filesystem::path(file_name);

    // Check if file already exists
    if (std::filesystem::exists(output_path))
    {
        throw ar::error::io_error(file_path, "Unable to copy file to '" + output_path.string() +
                                             "' as it already exists");
    }

    try
    {
        std::filesystem::copy_file(file_path, output_path);
    }
    catch (std::filesystem::filesystem_error& ex)
    {
        throw ar::error::io_error(file_path, "Unable to copy file to '" + output_path.string() +
                                             "' due to error: " + ex.what());
    }
}
