/**
 * @file emulator/ram-memory.hpp
 */

#ifndef ACCESS_TO_RETRO_RAM_MEMORY_HPP
#define ACCESS_TO_RETRO_RAM_MEMORY_HPP

#include <access-to-retro-dev/access-to-retro-dev.h>
#include <cstdlib>
#include <array>

namespace ar::chip8
{
    /// @brief CHIP8's font data, this array needs to be loaded to memory at 0x050
    constexpr std::array<ar_byte, 80> FONTSET =
                                              {
                                                      0xF0, 0x90, 0x90, 0x90, 0xF0,
                                                      0x20, 0x60, 0x20, 0x20, 0x70,
                                                      0xF0, 0x10, 0xF0, 0x80, 0xF0,
                                                      0xF0, 0x10, 0xF0, 0x10, 0xF0,
                                                      0x90, 0x90, 0xF0, 0x10, 0x10,
                                                      0xF0, 0x80, 0xF0, 0x10, 0xF0,
                                                      0xF0, 0x80, 0xF0, 0x90, 0xF0,
                                                      0xF0, 0x10, 0x20, 0x40, 0x40,
                                                      0xF0, 0x90, 0xF0, 0x90, 0xF0,
                                                      0xF0, 0x90, 0xF0, 0x10, 0xF0,
                                                      0xF0, 0x90, 0xF0, 0x90, 0x90,
                                                      0xE0, 0x90, 0xE0, 0x90, 0xE0,
                                                      0xF0, 0x80, 0x80, 0x80, 0xF0,
                                                      0xE0, 0x90, 0x90, 0x90, 0xE0,
                                                      0xF0, 0x80, 0xF0, 0x80, 0xF0,
                                                      0xF0, 0x80, 0xF0, 0x80, 0x80,
                                              };

    /// @brief Represents the size of CHIP8 RAM - 4kb
    constexpr std::size_t RAM_SIZE = 4096;

    /// @brief Represents random access memory of CHIP8
    class ram_memory
    {
    public:
        ram_memory();

        /**
         * @brief Loads a game ROM into the memory
         * @param executable Access to retro library executable
         */
        void load_binary(ar_executable* executable);

        /**
         * @brief Writes a byte to memory
         * @param addr Address where the value will be written to
         * @param value Value that will be written
         */
        void write(uint16_t addr, ar_byte value);

        /**
         * @brief Reads a byte from memory
         * @param addr Address of the requested memory location
         * @return
         */
        [[nodiscard]] ar_byte read(uint16_t addr) const;

        /**
         * @brief Reads memory address (and the one straight after it) at address to which program counter is set and
         *        returns an instruction from that address
         * @param pc_value Value of program counter
         * @return Encoded instruction
         */
        [[nodiscard]] uint16_t read_instruction(uint16_t pc_value) const;


    private:
        /// @brief Raw representation of the memory using an array
        std::array<ar_byte, ar::chip8::RAM_SIZE> _raw_memory { 0 };
    };
}

#endif //ACCESS_TO_RETRO_RAM_MEMORY_HPP
