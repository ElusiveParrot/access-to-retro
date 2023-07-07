/**
 * @file emulator/controller.hpp
 */

#ifndef ACCESS_TO_RETRO_CONTROLLER_HPP
#define ACCESS_TO_RETRO_CONTROLLER_HPP

#include <access-to-retro-dev/access-to-retro-dev.h>
#include <array>

namespace ar::chip8
{
    /**
     * @brief Represents CHIP8's controller keys
     * @details It looks exactly like a numpad on keyboards with an extra column (with keys numbered 0-F [hex])
     */
    enum class key
    {
        key_0,
        key_1,
        key_2,
        key_3,
        key_4,
        key_5,
        key_6,
        key_7,
        key_8,
        key_9,
        key_a,
        key_b,
        key_c,
        key_d,
        key_e,
        key_f
    };

    /// @brief Represents the number of keys that CHIP8 controller has
    constexpr unsigned KEY_COUNT = 16;

    /// @brief Class representing CHIP8's controller
    class controller
    {
    public:
        /**
         * @brief Setter for key status
         * @param key Button that was pressed
         * @param status Status of the key
         */
        void set_key_status(ar::chip8::key key, ar_unified_controller_key_status status);

        /**
         * @brief Check whether the key was pressed
         * @param key Key to check
         * @return Whether the key was pressed
         */
        bool is_key_pressed(ar::chip8::key key);

    private:
        /// @brief Maps 1:1 with 'ar::chip8::key' when key is casted to int, value represents whether it's pressed
        std::array<bool, ar::chip8::KEY_COUNT> _key_pad {};
    };
}

#endif //ACCESS_TO_RETRO_CONTROLLER_HPP
