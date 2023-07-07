/**
 * @file emulator/gpu.hpp
 */

#ifndef ACCESS_TO_RETRO_GPU_HPP
#define ACCESS_TO_RETRO_GPU_HPP

#include <SDL.h>
#include "frame-buffer.hpp"
#include "ram-memory.hpp"

namespace ar::chip8
{
    /// @brief Class representing CHIP8's GPU emulator
    class gpu
    {
    public:
        /**
         * @brief Default constructor
         * @param ram_link Link to the RAM memory
         */
        explicit gpu(ar::chip8::ram_memory& ram_link);
        ~gpu();

        /// @brief Render internal GPU state to the screen
        void render();

        /**
         * @brief Setter for draw flag
         * @param new_value Whether next GPU tick should render to the screen
         */
        void set_draw_flag(bool new_value);

        /**
         * @brief Getter for draw flag
         * @return Whether this GPU tick should render to the screen
         */
        [[nodiscard]] bool get_draw_flag() const;

        /******************* Instructions Functions *******************/

        /// @brief Clear the screen and set draw flag so that the screen updates to empty
        void clear_screen();

        /**
         * @brief Draw a sprite on the screen
         * @param x X coordinate where the sprite should be drawn
         * @param y Y coordinate where the sprite should be drawn
         * @param height Height of the sprite
         * @param register_i_value Value of special register I
         * @return Value to which general register 0xF (VF) should be set
         */
        [[nodiscard]] ar_byte draw(ar_byte x, ar_byte y, ar_byte height, uint16_t register_i_value);

    private:
        /// @brief Reference to RAM so that CPU can access it
        ar::chip8::ram_memory& _ram_link;

        /// @brief CHIP8's screen frame buffer
        ar::chip8::frame_buffer _frame_buffer {};

        /// @brief Indicates whether next GPU tick should render to the screen
        bool _draw_flag = false;

        /// @brief SDL's texture object to render framebuffer to
        SDL_Texture* _frame_buffer_texture = nullptr;

        /**
         * @brief Updates frame buffer texture with frame buffer pixels
         */
        void update_texture_with_frame_buffer();
    };
}

#endif //ACCESS_TO_RETRO_GPU_HPP
