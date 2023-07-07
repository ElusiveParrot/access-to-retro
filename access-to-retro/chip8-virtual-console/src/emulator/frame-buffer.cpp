#include "frame-buffer.hpp"

ar::chip8::frame_buffer::frame_buffer()
{
    clear();
}

void ar::chip8::frame_buffer::clear()
{
    for (uint32_t i = 0; i < (ar::chip8::SCREEN_RESOLUTION_X * ar::chip8::SCREEN_RESOLUTION_Y); i++)
    {
        _raw_pixels[i] = ar_pixel { 0, 0, 0, 255 };
    }
}

void ar::chip8::frame_buffer::flip_pixel_channels(unsigned x, unsigned y)
{
    ar_pixel& target_pixel = get_pixel_at(x, y);

    // Flip value of each colour channel
    target_pixel.r = target_pixel.r == PIXEL_TURNED_ON_CHANNEL_VALUE ? PIXEL_TURNED_OFF_CHANNEL_VALUE
                                                                     : PIXEL_TURNED_ON_CHANNEL_VALUE;
    target_pixel.g = target_pixel.g == PIXEL_TURNED_ON_CHANNEL_VALUE ? PIXEL_TURNED_OFF_CHANNEL_VALUE
                                                                     : PIXEL_TURNED_ON_CHANNEL_VALUE;
    target_pixel.b = target_pixel.b == PIXEL_TURNED_ON_CHANNEL_VALUE ? PIXEL_TURNED_OFF_CHANNEL_VALUE
                                                                     : PIXEL_TURNED_ON_CHANNEL_VALUE;
}

ar_pixel& ar::chip8::frame_buffer::get_pixel_at(unsigned x, unsigned y)
{
    std::size_t index = (y % ar::chip8::SCREEN_RESOLUTION_Y) *
                        ar::chip8::SCREEN_RESOLUTION_X + (x % ar::chip8::SCREEN_RESOLUTION_X);

    return _raw_pixels.at(index);
}

std::array<ar_pixel, ar::chip8::SCREEN_RESOLUTION_X * ar::chip8::SCREEN_RESOLUTION_Y>&
ar::chip8::frame_buffer::get_pixels()
{
    return _raw_pixels;
}
