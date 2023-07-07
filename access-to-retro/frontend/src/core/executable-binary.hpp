/**
 * @file core/executable-binary.hpp
 */

#ifndef ACCESS_TO_RETRO_EXECUTABLE_BINARY_HPP
#define ACCESS_TO_RETRO_EXECUTABLE_BINARY_HPP

#include <fstream>
#include <string>
#include "helpers/types.hpp"

namespace ar::core
{
    /// @brief Represents a binary file that is able to be executed by virtual console, usually a game
    class executable_binary
    {
    public:
        /**
         * @brief Default constructor
         * @param path Path to the executable
         * @note File content are not loaded into memory in the constructor, use 'load_to_memory' for that
         * @warning If file is accessed before 'load_to_memory' is called then exception will be thrown
         * @throws Exceptions:
         *  - ar::error::io_error: File not found
         */
        explicit executable_binary(std::string path);

        /**
         * @brief Loads contents of the file to memory, allows the file to be accessed
         * @throws Exceptions:
         *  - ar::error:io_error: Unable to read file
         */
        void load_to_memory();

        /// @brief Unloads the file from memory
        void unload_file();

        /**
         * @brief Read multiple bytes from the binary file
         * @param addr Starting address of the read
         * @param size Number of bytes to read
         * @throws Exceptions:
         *  - std::runtime_error: Attempted to read from file before using 'load_to_memory'
         *  - ar::error::io_error: Invalid read address
         * @return Bytes at provided address
         */
        [[nodiscard]] std::vector<ar::types::byte> read_bytes(uint64_t addr, std::size_t size) const;

        /**
         * @brief Read a single byte from the binary file
         * @param addr Address of the byte
         * @throws Exceptions:
         *  - std::runtime_error: Attempted to read from file before using 'load_to_memory'
         *  - ar::error::io_error: Invalid read address
         * @return Byte at provided address
         */
        [[nodiscard]] ar::types::byte read_byte(uint64_t addr) const;

        /**
         * @brief Get the path which this executable represents
         * @return Path of the binary
         */
        [[nodiscard]] std::string get_path() const;

        /**
         * @brief Get the size of the represented file
         * @return Size of the loaded file
         */
        [[nodiscard]] uint64_t get_file_size() const;

        /**
         * @brief Get the raw bytes of the represented file
         * @return Bytes representing file contents
         */
        [[nodiscard]] const ar::types::byte* get_file_raw_bytes() const;

        /**
         * @brief Get the name of the file itself from it's path
         * @return Name of the file
         */
        [[nodiscard]] std::string get_file_name() const;

        /**
         * @brief Get the name of the file extension from it's path
         * @return Extension of the file
         */
        [[nodiscard]] std::string get_file_extension() const;

    private:
        /// @brief Represented file path
        std::string _path;

        /// @brief Size of the represented file
        uint64_t _file_size;

        /// @brief Whether the file was loaded into memory using 'load_to_memory'
        bool _loaded_to_memory = false;

        /// @brief Contains content of the file after using 'load_to_memory'
        std::vector<ar::types::byte> _file_content;
    };
}

#endif //ACCESS_TO_RETRO_EXECUTABLE_BINARY_HPP
