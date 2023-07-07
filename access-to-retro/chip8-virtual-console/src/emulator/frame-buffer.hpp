#ifndef ACCESS_TO_RETRO_FRAME_BUFFER_HPP
#define ACCESS_TO_RETRO_FRAME_BUFFER_HPP

#include <access-to-retro-dev/access-to-retro-dev.h>
#include <cstdint>
#include <array>

namespace ar::chip8
{
    /// @brief CHIP8's screen resolution width
    constexpr uint32_t SCREEN_RESOLUTION_X = 64;

    /// @brief CHIP8's screen resolution height
    constexpr uint32_t SCREEN_RESOLUTION_Y = 32;

    /// @brief Value of colour channels when pixel is turned off
    constexpr ar_byte PIXEL_TURNED_OFF_CHANNEL_VALUE = 0x00;

    /// @brief Value of colour channels when pixel is turned on
    constexpr ar_byte PIXEL_TURNED_ON_CHANNEL_VALUE = 0xFF;

    /// @brief Represents a CHIP8's internal frame buffer object
    class frame_buffer
    {
    public:
        frame_buffer();

        /**
         * @brief Flip colour channels in individual pixel, CHIP8 is black and white so no need to set individual values
         * @param x X position of the pixel
         * @param y Y position of the pixel
         */
        void flip_pixel_channels(unsigned x, unsigned y);

        /// @brief Clears the frame buffer by setting each pixel to rgba(0,0,0,255)
        void clear();

        /**
         * @brief Getter for pixel arrays
         * @return Raw pixel array
         */
        [[nodiscard]] std::array<ar_pixel, ar::chip8::SCREEN_RESOLUTION_X * ar::chip8::SCREEN_RESOLUTION_Y>&
        get_pixels();

        /**
         * @brief Getter for a specific pixel
         * @param x X position of the pixel
         * @param y Y position of the pixel
         * @return Targeted pixel
         */
        ar_pixel& get_pixel_at(unsigned x, unsigned y);

    private:
        /// @brief Array of raw pixel colours
        std::array<ar_pixel, ar::chip8::SCREEN_RESOLUTION_X * ar::chip8::SCREEN_RESOLUTION_Y> _raw_pixels {};
    };
}

#endif //ACCESS_TO_RETRO_FRAME_BUFFER_HPP
