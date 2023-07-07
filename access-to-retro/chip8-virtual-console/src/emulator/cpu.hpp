/**
 * @file emulator/cpu.hpp
 */

#ifndef ACCESS_TO_RETRO_CPU_HPP
#define ACCESS_TO_RETRO_CPU_HPP

#include <access-to-retro-dev/access-to-retro-dev.h>
#include <array>
#include <stack>
#include "controller.hpp"
#include "ram-memory.hpp"
#include "gpu.hpp"

namespace ar::chip8
{
    /// @brief Represents a number of general registers that CHIP8 cpu has
    constexpr std::size_t GENERAL_REGISTER_COUNT = 16;

    /// @brief Class representing CHIP8's CPU emulator
    class cpu
    {
    public:
        /**
         * @brief Default constructor
         * @param ram_link Link to the RAM object
         * @param gpu_link Link to the GPU object
         * @param controller_link Link to the controller object
         */
        explicit cpu(ar::chip8::ram_memory& ram_link, ar::chip8::gpu& gpu_link, ar::chip8::controller& controller_link);

        /// @brief Indicate that timers should be updated (should update at 60hz)
        void tick_timers();

        /// @brief Fetch - decode - execute a single instruction (should run at 600hz [cpu clock speed])
        void tick();

    private:
        /// @brief Reference to GPU so that CPU can access and control it
        ar::chip8::gpu& _gpu_link;

        /// @brief Reference to RAM so that CPU can access it
        ar::chip8::ram_memory& _ram_link;

        /// @brief Reference to controller so that CPU can access and control it
        ar::chip8::controller& _controller_link;

        /// @brief Stack for function calls
        std::stack<uint16_t> _call_stack {};

        /// @brief Last encoded instruction (opcode)
        uint16_t _encoded_instruction = 0x0000;

        // ****************** Registers ******************

        /// @brief General registers, used by game developers for anything they want, they are also called V0...VF
        std::array<ar_byte, ar::chip8::GENERAL_REGISTER_COUNT> _general_registers { 0 };

        /**
         * @brief Program Counter - special register that tracks the address of current instruction
         * @remark CHIP8 always starts execution at 0x200 address, right after font area of memory
         */
        uint16_t _special_register_pc = 0x200;

        /// @brief Special register called 'address register', used by several instructions as a memory pointer
        uint16_t _special_register_i = 0x0000;

        // ****************** Timers ******************

        /// @brief Counts down at 60hz until 0. When the value reaches 0 a BEEP sound is played
        ar_byte _sound_timer = 0x00;

        /// @brief Counts down at 60hz until 0. Used by games to time events, can be set and read by instructions
        ar_byte _delay_timer = 0x00;

        // ****************** CPU Tick Functions ******************

        /// @brief Increments program counter to the next instruction after fetch has completed
        void increment_program_counter();

        /// @brief Fetch parth of fetch - decode - execute loop
        void fetch();

        /// @brief Decode and execute part of fetch - decode - execute loop
        void decode_and_execute();

        /******************* Instructions Functions *******************/

        /**
         * @brief Get argument X from the opcode
         * @return Value of argument at position 0x0X00
         */
        [[nodiscard]] ar_byte get_x_argument_from_opcode();

        /**
         * @brief Get argument Y from the opcode
         * @return Value of argument at position 0x00Y0
         */
        [[nodiscard]] ar_byte get_y_argument_from_opcode();

        /**
         * @brief Get argument NNN from the opcode
         * @return Value of argument at position 0x0NNN
         */
        [[nodiscard]] uint16_t get_nnn_argument_from_opcode();

        /**
         * @brief Get argument NN from the opcode
         * @return Value of argument at position 0x00NN
         */
        [[nodiscard]] ar_byte get_nn_argument_from_opcode();

        /**
         * @brief Get argument N from the opcode
         * @return Value of argument at position 0x000N
         */
        [[nodiscard]] ar_byte get_n_argument_from_opcode();

        // Note: VX = General register with index in argument X

        /// @brief Opcode 0x00E0 -> Send clear screen job to GPU
        void clear_screen();

        /// @brief Opcode 0x00EE -> Return from a function
        void fn_return();

        /// @brief Opcode 0x1NNN -> Jump to address NNN
        void jump();

        /// @brief Opcode 0x2NNN, 0x0NNN -> Call function at NNN
        void fn_call();

        /// @brief Opcode 0x3XNN -> Skip next instruction if value in if VX == NN
        void skip_if_vx_eq_nn();

        /// @brief Opcode 0x4XNN -> Skip next instruction if value in VX != NN
        void skip_if_vx_neq_nn();

        /// @brief Opcode 0x5XN0 -> Skip next instruction if value in VX == value in VY
        void skip_if_vx_eq_vy();

        /// @brief Opcode 0x6XNN -> Set VX to NN
        void set_vx_to_nn();

        /// @brief Opcode 0x7XNN -> Add NN to VX
        void set_add_nn_to_vx();

        /// @brief Opcode 0x8XY0 -> Set VX to value in VY
        void set_vx_to_vy();

        /// @brief Opcode 0x8XY1 -> Set VX to VX | VY
        void set_vx_to_vx_or_vy();

        /// @brief Opcode 0x8XY2 -> Set VX to VX & VY
        void set_vx_to_vx_and_vy();

        /// @brief Opcode 0x8XY3 -> Set VX to VX ~ VY
        void set_vx_to_vx_xor_vy();

        /// @brief Opcode 0x8XY4 -> Add value in VY to VX
        void add_vy_to_vx();

        /// @brief Opcode 0x8XY5 -> Subtract value in VY from VX
        void sub_vy_from_vx();

        /// @brief Opcode 0x8XY6 -> Store least significant bit from value in VX in VF and shift VX to the right by 1
        void store_least_sig_vx();

        /// @brief Opcode 0x8XY7 -> Set VX to value in VY minus value in VX
        void set_vx_to_vy_sub_vx();

        /// @brief Opcode 0x8XYE -> Store most significant bit from value in VX in VF and shift VX to the left by 1
        void store_most_sig_vx();

        /// @brief Opcode 0x9XY0 -> Skip next instruction if value in VX != value in VY
        void skip_if_vx_neq_vy();

        /// @brief Opcode 0xANNN -> Set special register I to NNN
        void set_i_to_nnn();

        /// @brief Opcode 0xBNNN -> Jump to address (NNN + value in V0)
        void jump_add_v0();

        /// @brief Opcode 0xCXNN -> Set VX to NN & Random number
        void set_vx_to_rand_and_nn();

        /// @brief Opcode 0xDXYN -> Send draw job to GPU
        void draw();

        /// @brief Opcode 0xEX9E -> Skip next instruction if key with index stored in VX is pressed
        void skip_if_vx_key_pressed();

        /// @brief Opcode 0xEXA1 -> SKip next instruction if key with index stored in VX is NOT pressed
        void skip_if_vx_key_not_pressed();

        /// @brief Opcode 0xFX07 -> Set VX to value of delay timer
        void set_vx_to_delay_timer();

        /// @brief Opcode 0xFX0A -> Wait for key press and store its index in VX
        void set_vx_to_wait_get_key();

        /// @brief Opcode 0xFX15 -> Set delay timer to value in VX
        void set_delay_timer_to_vx();

        /// @brief Opcode 0xFX18 -> Set sound timer to value in VX
        void set_sound_timer_to_vx();

        /// @brief Opcode 0xFX1E -> Add VX to special register I
        void add_vx_to_i();

        /// @brief Opcode 0xFX29 -> Set I to location of the sprite for the font character with index stored in VX
        void set_i_to_sprite_location_for_vx();

        /// @brief Opcode 0xFX33 -> Stored binary-coded decimal representation of value in VX at address in I
        void store_vcx_bcd_at_i();

        /// @brief Opcode 0xFX55 -> Dump values in general registers from V0 to VX to memory starting at address in I
        void dump_general_registers_at_i();

        /// @brief Opcode 0xFX65 -> Restore values from general registers from V0 to VX from memory starting at I
        void fill_general_registers_from_i();
    };
}


#endif //ACCESS_TO_RETRO_CPU_HPP
