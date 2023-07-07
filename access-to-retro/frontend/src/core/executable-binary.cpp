#include <filesystem>
#include <utility>
#include "error/io-error.hpp"
#include "util/logger.hpp"
#include "executable-binary.hpp"

ar::core::executable_binary::executable_binary(std::string path) :
        _path(std::move(path))
{
    std::ifstream file(_path.c_str(), std::ios::binary);
    if (!file.good())
    {
        throw ar::error::io_error(_path, "Binary file is not valid");
    }

    file.unsetf(std::ios::skipws);

    // Set pointer to the end of the file
    file.seekg(0, std::ios::end);

    // Get file size
    _file_size = static_cast<uint64_t>(file.tellg());

    // Rewind pointer to the beginning of the file
    file.seekg(0, std::ios::beg);

    file.close();

    LOG_DEBUG("core.executable_binary", "Executable binary object with name '" + _path + "' created");
}

void ar::core::executable_binary::load_to_memory()
{
    std::ifstream file(_path.c_str(), std::ios::binary);

    // Unset SKIPWS flag
    file.unsetf(std::ios::skipws);

    if (!file.good())
    {
        throw ar::error::io_error(_path, "Binary file is not valid");
    }

    _file_content.reserve(_file_size);

    try
    {
        _file_content.insert(_file_content.begin(),
                             std::istream_iterator<ar::types::byte>(file),
                             std::istream_iterator<ar::types::byte>());
    }
    catch (const std::exception&)
    {
        throw ar::error::io_error(_path, "Unable to read binary file");
    }

    file.close();

    _loaded_to_memory = true;
}

void ar::core::executable_binary::unload_file()
{
    _loaded_to_memory = false;
    _file_content.clear();
}

std::vector<ar::types::byte> ar::core::executable_binary::read_bytes(uint64_t addr, std::size_t size) const
{
    if (!_loaded_to_memory)
    {
        throw std::runtime_error("Attempted to read from binary file at '" + _path + "' before loading it to memory");
    }

    // Address of last byte that will be read
    uint64_t last_read_addr = addr + size - 1;
    if (last_read_addr >= _file_size)
    {
        throw ar::error::io_error(_path, "Read address outside of the file");
    }

    std::vector<ar::types::byte> bytes {};
    for (uint64_t                i = addr; i < last_read_addr; i++)
    {
        bytes.push_back(read_byte(i));
    }

    return bytes;
}

ar::types::byte ar::core::executable_binary::read_byte(uint64_t addr) const
{
    if (!_loaded_to_memory)
    {
        throw std::runtime_error("Attempted to read from binary file at '" + _path + "' before loading it to memory");
    }

    if (addr >= _file_size)
    {
        throw ar::error::io_error(_path, "Read address outside of the file");
    }

    return _file_content[addr];
}

std::string ar::core::executable_binary::get_path() const
{
    return _path;
}

uint64_t ar::core::executable_binary::get_file_size() const
{
    return _file_size;
}

const ar::types::byte* ar::core::executable_binary::get_file_raw_bytes() const
{
    return _file_content.data();
}

std::string ar::core::executable_binary::get_file_name() const
{
    return std::filesystem::path(_path).stem().string();
}

std::string ar::core::executable_binary::get_file_extension() const
{
    return std::filesystem::path(_path).extension().string();
}

