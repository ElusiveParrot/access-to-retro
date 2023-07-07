#include "ram-memory.hpp"

ar::chip8::ram_memory::ram_memory()
{
    // Load fontset at 0x050
    for (std::size_t i = 0; i < ar::chip8::FONTSET.size(); i++)
    {
        _raw_memory[i + 0x050] = ar::chip8::FONTSET[i];
    }
}

void ar::chip8::ram_memory::load_binary(ar_executable* executable)
{
    // Load binary at 0x200
    for (std::size_t i = 0; i < executable->size; i++)
    {
        _raw_memory[i + 0x200] = executable->raw_bytes[i];
    }
}

void ar::chip8::ram_memory::write(uint16_t addr, ar_byte value)
{
    _raw_memory[addr] = value;
}

ar_byte ar::chip8::ram_memory::read(uint16_t addr) const
{
    return _raw_memory[addr];
}

uint16_t ar::chip8::ram_memory::read_instruction(uint16_t pc_value) const
{
    // Combine bytes at PC and PC+1 to construct an instruction
    return static_cast<uint16_t>((read(pc_value) << 8) | read(pc_value + 1));
}
