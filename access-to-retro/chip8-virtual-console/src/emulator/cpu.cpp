#include <ctime>
#include "cpu.hpp"

ar::chip8::cpu::cpu(ar::chip8::ram_memory& ram_link, ar::chip8::gpu& gpu_link, ar::chip8::controller& controller_link) :
        _gpu_link(gpu_link),
        _ram_link(ram_link),
        _controller_link(controller_link)
{

}

void ar::chip8::cpu::tick_timers()
{
    // If timer is above 0 then just subtract 1, otherwise do nothing

    if (_delay_timer > 0)
    {
        _delay_timer--;
    }

    if (_sound_timer > 0)
    {
        _sound_timer--;
    }
}

void ar::chip8::cpu::tick()
{
    // Fetch...
    fetch();

    // ... decode and execute.
    decode_and_execute();
}

void ar::chip8::cpu::increment_program_counter()
{
    // CHIP8 instructions are 16bit long so add two bytes
    _special_register_pc += 2;
}

void ar::chip8::cpu::fetch()
{
    // Opcode is en encoded instruction, in CHIP8 opcodes are 16bit long
    _encoded_instruction = _ram_link.read_instruction(_special_register_pc);

    // Move program counter to next instruction
    increment_program_counter();
}

void ar::chip8::cpu::decode_and_execute()
{
    /*
     * For comments with format for example: 'Opcode 0xDXYN':
     *
     * This means that the half-byte at position X is an argument, same for Y and N.
     *
     * Example:
     * The opcode is 0xD05C:
     *  - Argument X = 0x0
     *  - Argument Y = 0x5
     *  - Argument N = 0xC
     */

    // Bitwise AND the opcode with 0xF000 mask to get 1st hex digit (for ex, for opcode 0xABCD this will return 0xA000)
    switch (_encoded_instruction & 0xF000)
    {
        // Opcode starts with 0, for example: 0x0ABC
        case 0x0000:
            // Multiple opcodes start with 0 so confirm which one it is exactly
            switch (_encoded_instruction)
            {
                // Opcode: 0x00E0
                case 0x00E0:
                    clear_screen();
                    break;

                    // Opcode: 0x00EE
                case 0x00EE:
                    fn_return();
                    break;

                    // Opcode: 0x0NNN
                default:
                    fn_call();
                    break;

            }
            break;

            // Opcode 0x1NNN
        case 0x1000:
            jump();
            break;

            // Opcode 0x2NNN
        case 0x2000:
            fn_call();
            break;

            // Opcode 0x3XNN
        case 0x3000:
            skip_if_vx_eq_nn();
            break;

            // Opcode 0x4XNN
        case 0x4000:
            skip_if_vx_neq_nn();
            break;

            // Opcode 0x5XY0
        case 0x5000:
            skip_if_vx_eq_vy();
            break;

            // Opcode 0x6XNN
        case 0x6000:
            set_vx_to_nn();
            break;

            // Opcode 0x7XNN
        case 0x7000:
            set_add_nn_to_vx();
            break;

            // Opcode starts with 8 for example 0x8ABC
        case 0x8000:
            // Multiple opcodes start with 0 so confirm which one it is exactly based on last half-byte
            switch (_encoded_instruction & 0x000F)
            {
                // Opcode 0x8XY0
                case 0x0:
                    set_vx_to_vy();
                    break;

                    // Opcode 0x8XY1
                case 0x1:
                    set_vx_to_vx_or_vy();
                    break;

                    // Opcode 0x8XY2
                case 0x2:
                    set_vx_to_vx_and_vy();
                    break;

                    // Opcode 0x8XY3
                case 0x3:
                    set_vx_to_vx_xor_vy();
                    break;

                    // Opcode 0x8XY4
                case 0x4:
                    add_vy_to_vx();
                    break;

                    // Opcode 0x8XY5
                case 0x5:
                    sub_vy_from_vx();
                    break;

                case 0x6:
                    // Opcode 0x8XY6
                    store_least_sig_vx();
                    break;

                    // Opcode 0x8XY7
                case 0x7:
                    set_vx_to_vy_sub_vx();
                    break;

                    // Opcode 0x8XYE
                case 0xE:
                    store_most_sig_vx();
                    break;

            }
            break;

            // Opcode 0x9XY0
        case 0x9000:
            skip_if_vx_neq_vy();
            break;

            // Opcode 0xANNN
        case 0xA000:
            set_i_to_nnn();
            break;

            // Opcode 0xBNNN
        case 0xB000:
            jump_add_v0();
            break;

            // Opcode 0xCXNN
        case 0xC000:
            set_vx_to_rand_and_nn();
            break;

            // Opcode 0xDXYN
        case 0xD000:
            draw();
            break;

            // Opcode starting with E for ex: 0xEABC
        case 0xE000:
            // Multiple opcodes start with E so confirm which one it is based on last byte
            switch (_encoded_instruction & 0x00FF)
            {
                // Opcode 0xEX9E
                case 0x9E:
                    skip_if_vx_key_pressed();
                    break;

                    // Opcode 0xEXA1
                case 0xA1:
                    skip_if_vx_key_not_pressed();
                    break;
            }
            break;

            // Opcode starting with F for ex: 0xF00
        case 0xF000:
            // Multiple opcodes start with F so confirm which one it is based on last byte
            switch (_encoded_instruction & 0x00FF)
            {
                // Opcode 0xFX07
                case 0x07:
                    set_vx_to_delay_timer();
                    break;

                    // Opcode 0xFX0A
                case 0x0A:
                    set_vx_to_wait_get_key();
                    break;

                    // Opcode 0xFX15
                case 0x15:
                    set_delay_timer_to_vx();
                    break;

                    // Opcode 0xFX18
                case 0x18:
                    set_sound_timer_to_vx();
                    break;

                    // Opcode 0xFX1E
                case 0x1E:
                    add_vx_to_i();
                    break;

                    // Opcode 0xFX29
                case 0x29:
                    set_i_to_sprite_location_for_vx();
                    break;

                    // Opcode 0xFX33
                case 0x33:
                    store_vcx_bcd_at_i();
                    break;

                    // Opcode 0xFX55
                case 0x55:
                    dump_general_registers_at_i();
                    break;

                    // Opcode 0xFX65
                case 0x65:
                    fill_general_registers_from_i();
                    break;
            }
            break;
    }
}

/****************************************************************************************************
 Instruction Functions
****************************************************************************************************/

ar_byte ar::chip8::cpu::get_x_argument_from_opcode()
{
    return (_encoded_instruction & 0x0F00) >> 8;
}

ar_byte ar::chip8::cpu::get_y_argument_from_opcode()
{
    return (_encoded_instruction & 0x00F0) >> 4;
}

uint16_t ar::chip8::cpu::get_nnn_argument_from_opcode()
{
    return _encoded_instruction & 0x0FFF;
}

ar_byte ar::chip8::cpu::get_nn_argument_from_opcode()
{
    return _encoded_instruction & 0x00FF;
}

ar_byte ar::chip8::cpu::get_n_argument_from_opcode()
{
    return _encoded_instruction & 0x000F;
}

void ar::chip8::cpu::clear_screen()
{
    // Send job to the GPU
    _gpu_link.clear_screen();
}

void ar::chip8::cpu::fn_return()
{
    // Get new program counter value from the top of the call stack
    _special_register_pc = _call_stack.top();
    _call_stack.pop();
}

void ar::chip8::cpu::jump()
{
    // Absolute jump so set the value of PC directly
    _special_register_pc = get_nnn_argument_from_opcode();
}

void ar::chip8::cpu::fn_call()
{
    // Add current PC to the top of the stack
    _call_stack.push(_special_register_pc);

    // Set PC to new address
    _special_register_pc = get_nnn_argument_from_opcode();
}

void ar::chip8::cpu::skip_if_vx_eq_nn()
{
    ar_byte register_index = get_x_argument_from_opcode();
    ar_byte nn_value       = get_nn_argument_from_opcode();

    if (_general_registers[register_index] == nn_value)
    {
        // Increment program counter again to skip next instruction
        increment_program_counter();
    }
}

void ar::chip8::cpu::skip_if_vx_neq_nn()
{
    ar_byte register_index = get_x_argument_from_opcode();
    ar_byte nn_value       = get_nn_argument_from_opcode();

    if (_general_registers[register_index] != nn_value)
    {
        // Increment program counter again to skip next instruction
        increment_program_counter();
    }
}

void ar::chip8::cpu::skip_if_vx_eq_vy()
{
    ar_byte register_index_x = get_x_argument_from_opcode();
    ar_byte register_index_y = get_y_argument_from_opcode();

    if (_general_registers[register_index_x] == _general_registers[register_index_y])
    {
        // Increment program counter again to skip next instruction
        increment_program_counter();
    }
}

void ar::chip8::cpu::set_vx_to_nn()
{
    ar_byte register_index_x = get_x_argument_from_opcode();

    _general_registers[register_index_x] = get_nn_argument_from_opcode();
}

void ar::chip8::cpu::set_add_nn_to_vx()
{
    ar_byte register_index_x = get_x_argument_from_opcode();

    _general_registers[register_index_x] += get_nn_argument_from_opcode();
}

void ar::chip8::cpu::set_vx_to_vy()
{
    ar_byte register_index_x = get_x_argument_from_opcode();
    ar_byte register_index_y = get_y_argument_from_opcode();

    _general_registers[register_index_x] = _general_registers[register_index_y];
}

void ar::chip8::cpu::set_vx_to_vx_or_vy()
{
    ar_byte register_index_x = get_x_argument_from_opcode();
    ar_byte register_index_y = get_y_argument_from_opcode();

    _general_registers[register_index_x] |= _general_registers[register_index_y];
}

void ar::chip8::cpu::set_vx_to_vx_and_vy()
{
    ar_byte register_index_x = get_x_argument_from_opcode();
    ar_byte register_index_y = get_y_argument_from_opcode();

    _general_registers[register_index_x] &= _general_registers[register_index_y];
}

void ar::chip8::cpu::set_vx_to_vx_xor_vy()
{
    ar_byte register_index_x = get_x_argument_from_opcode();
    ar_byte register_index_y = get_y_argument_from_opcode();

    _general_registers[register_index_x] ^= _general_registers[register_index_y];
}

void ar::chip8::cpu::add_vy_to_vx()
{
    ar_byte register_index_x = get_x_argument_from_opcode();
    ar_byte register_index_y = get_y_argument_from_opcode();

    uint16_t add_res = _general_registers[register_index_x] + _general_registers[register_index_y];

    _general_registers[register_index_x] = static_cast<ar_byte>(add_res);

    // Set VF to whether the result overflows byte
    _general_registers[0xF] = add_res > 0xFF;
}

void ar::chip8::cpu::sub_vy_from_vx()
{
    ar_byte register_index_x = get_x_argument_from_opcode();
    ar_byte register_index_y = get_y_argument_from_opcode();

    uint16_t sub_res = _general_registers[register_index_x] - _general_registers[register_index_y];

    _general_registers[register_index_x] = static_cast<ar_byte>(sub_res % 0x100);

    // Set VF to whether the result overflow/underflow byte
    _general_registers[0xF] = (int16_t) (sub_res) >= 0x0;
}

void ar::chip8::cpu::store_least_sig_vx()
{
    ar_byte register_index_x = get_x_argument_from_opcode();

    // Store least significant bit in VF
    _general_registers[0xF] = (_general_registers[register_index_x] & 1) == 1;

    // Shift to the right by 1
    _general_registers[register_index_x] >>= 1;
}

void ar::chip8::cpu::set_vx_to_vy_sub_vx()
{
    ar_byte register_index_x = get_x_argument_from_opcode();
    ar_byte register_index_y = get_y_argument_from_opcode();

    uint16_t sub_res = _general_registers[register_index_y] - _general_registers[register_index_x];

    _general_registers[register_index_x] = static_cast<ar_byte>(sub_res % 0x100);

    // Set VF to whether the result overflow/underflow byte
    _general_registers[0xF] = (int16_t) (sub_res) >= 0x0;
}

void ar::chip8::cpu::store_most_sig_vx()
{
    ar_byte register_index_x = get_x_argument_from_opcode();

    // Get most significant bit
    _general_registers[0xF] = (_general_registers[register_index_x] & 0b10000000) == 0b10000000;

    // Shift left by 1
    _general_registers[register_index_x] <<= 1;
}

void ar::chip8::cpu::skip_if_vx_neq_vy()
{
    ar_byte register_index_x = get_x_argument_from_opcode();
    ar_byte register_index_y = get_y_argument_from_opcode();

    if (_general_registers[register_index_x] != _general_registers[register_index_y])
    {
        // Increment program counter again to skip next instruction
        increment_program_counter();
    }
}

void ar::chip8::cpu::set_i_to_nnn()
{
    _special_register_i = get_nnn_argument_from_opcode();
}

void ar::chip8::cpu::jump_add_v0()
{
    // Absolute jump so just set PC
    _special_register_pc = get_nnn_argument_from_opcode() + _general_registers[0];
}

void ar::chip8::cpu::set_vx_to_rand_and_nn()
{
    ar_byte register_index_x = get_x_argument_from_opcode();

    // Seed the number generator, security doesn't matter so time is fine as a seed
    srand(static_cast<unsigned>(time(nullptr)));

    _general_registers[register_index_x] = (rand() % 0x100) & get_nn_argument_from_opcode();
}

void ar::chip8::cpu::draw()
{
    // Get draw coordinates from registers
    ar_byte draw_at_x = _general_registers[get_x_argument_from_opcode()];
    ar_byte draw_at_y = _general_registers[get_y_argument_from_opcode()];

    // Height is encoded in the instruction
    ar_byte sprite_height = get_n_argument_from_opcode();

    // Send job to the GPU
    _general_registers[0xF] = _gpu_link.draw(draw_at_x, draw_at_y, sprite_height, _special_register_i);
}

void ar::chip8::cpu::skip_if_vx_key_pressed()
{
    auto key = static_cast<ar::chip8::key>(_general_registers[get_x_argument_from_opcode()]);

    if (_controller_link.is_key_pressed(key))
    {
        // Skip next instruction
        increment_program_counter();
    }
}

void ar::chip8::cpu::skip_if_vx_key_not_pressed()
{
    auto key = static_cast<ar::chip8::key>(_general_registers[get_x_argument_from_opcode()]);

    if (!_controller_link.is_key_pressed(key))
    {
        // Skip next instruction
        increment_program_counter();
    }
}

void ar::chip8::cpu::set_vx_to_delay_timer()
{
    ar_byte register_index_x = get_x_argument_from_opcode();

    _general_registers[register_index_x] = _delay_timer;
}

void ar::chip8::cpu::set_vx_to_wait_get_key()
{
    bool key_pressed = false;

    for (ar_byte i = 0; i < 16; i++)
    {
        if (_controller_link.is_key_pressed(static_cast<ar::chip8::key>(i)))
        {
            _general_registers[get_x_argument_from_opcode()] = i;
            key_pressed = true;
        }
    }

    // Key not pressed = run this instruction again
    if (!key_pressed)
    {
        _special_register_pc -= 2;
    }
}

void ar::chip8::cpu::set_delay_timer_to_vx()
{
    ar_byte register_index_x = get_x_argument_from_opcode();

    _delay_timer = _general_registers[register_index_x];
}

void ar::chip8::cpu::set_sound_timer_to_vx()
{
    ar_byte register_index_x = get_x_argument_from_opcode();

    _sound_timer = _general_registers[register_index_x];
}

void ar::chip8::cpu::add_vx_to_i()
{
    ar_byte register_index_x = get_x_argument_from_opcode();

    _special_register_i += _general_registers[register_index_x];
}

void ar::chip8::cpu::set_i_to_sprite_location_for_vx()
{
    ar_byte register_index_x = get_x_argument_from_opcode();

    _special_register_i = _general_registers[register_index_x] * 5;
}

void ar::chip8::cpu::store_vcx_bcd_at_i()
{
    ar_byte register_index_x = get_x_argument_from_opcode();
    ar_byte register_x_value = _general_registers[register_index_x];

    // Write BCD to memory
    _ram_link.write(_special_register_i, register_x_value / 100);
    _ram_link.write(_special_register_i + 1, (register_x_value / 10) % 10);
    _ram_link.write(_special_register_i + 2, register_x_value % 10);
}

void ar::chip8::cpu::dump_general_registers_at_i()
{
    ar_byte x_arg = get_x_argument_from_opcode();

    for (ar_byte i = 0; i <= x_arg; i++)
    {
        _ram_link.write(_special_register_i + i, _general_registers[i]);
    }
}

void ar::chip8::cpu::fill_general_registers_from_i()
{
    ar_byte x_arg = get_x_argument_from_opcode();

    for (ar_byte i = 0; i <= x_arg; i++)
    {
        _general_registers[i] = _ram_link.read(_special_register_i + i);
    }
}
