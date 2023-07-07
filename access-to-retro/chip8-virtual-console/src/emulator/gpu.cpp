#include <SDL.h>
#include "gpu.hpp"

ar::chip8::gpu::gpu(ar::chip8::ram_memory& ram_link) :
        _ram_link(ram_link)
{
    /*
     * Create new SDL texture that will be rendered to the screen, CHIP8's GPU framebuffer will be used to
     * render to this texture.
     *
     * Pixel format is set to ABGR8888 - 4 channels, 8 bit each. (a,b,g,r)
     *
     * Texture access set to STATIC for SDL_UpdateTexture to work
     *
     * Texture size set to CHIP8's screen resolution, this can then be scaled to the window using up-scaling
     */
    _frame_buffer_texture = SDL_CreateTexture(ar_graphics_get_sdl_renderer(), SDL_PIXELFORMAT_ABGR8888,
                                              SDL_TEXTUREACCESS_STATIC,
                                              ar::chip8::SCREEN_RESOLUTION_X, ar::chip8::SCREEN_RESOLUTION_Y);
}

ar::chip8::gpu::~gpu()
{
    SDL_DestroyTexture(_frame_buffer_texture);
}

void ar::chip8::gpu::render()
{
    update_texture_with_frame_buffer();

    SDL_Renderer* renderer = ar_graphics_get_sdl_renderer();

    // Clear screen -> Copy texture to renderer -> Show renderer on screen
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, _frame_buffer_texture, nullptr, nullptr);
    SDL_RenderPresent(renderer);
}

void ar::chip8::gpu::update_texture_with_frame_buffer()
{
    auto& pixels = _frame_buffer.get_pixels();

    // Create new texture pixel array. Texture is in format of ABGR8888, each pixel is a 32-bit integer
    uint32_t current_texture_pixel_array[ar::chip8::SCREEN_RESOLUTION_X * ar::chip8::SCREEN_RESOLUTION_Y] = { 0 };

    for (uint32_t i = 0; i < ar::chip8::SCREEN_RESOLUTION_X * ar::chip8::SCREEN_RESOLUTION_Y; i++)
    {
        ar_pixel& pixel = pixels.at(i);

        /*
         * Texture is in format of ABGR8888, each pixel is a 32-bit integer
         * For example: 0xDDCCBBAA where:
         *  - 0xDD is A
         *  - 0xCC is B
         *  - 0xBB is G
         *  - 0xAA is R
         *
         * This 32-bit integer can be constructed using ar_pixel object.
         *
         * Construct uint32_t from byte color channels using bitwise operations
         */

        uint32_t pixel_value = 0x00000000;

        pixel_value = pixel_value | (static_cast<uint32_t>(pixel.a << 24));
        pixel_value = pixel_value | (static_cast<uint32_t>(pixel.b << 16));
        pixel_value = pixel_value | (static_cast<uint32_t>(pixel.g << 8));
        pixel_value = pixel_value | (pixel.r);

        current_texture_pixel_array[i] = pixel_value;
    }

    SDL_UpdateTexture(_frame_buffer_texture, nullptr, &current_texture_pixel_array,
                      SCREEN_RESOLUTION_X * 4);
}

void ar::chip8::gpu::set_draw_flag(bool new_value)
{
    _draw_flag = new_value;
}

bool ar::chip8::gpu::get_draw_flag() const
{
    return _draw_flag;
}

void ar::chip8::gpu::clear_screen()
{
    // Clear the frame buffer
    _frame_buffer.clear();

    // Set the draw flag to update the screen
    set_draw_flag(true);
}

ar_byte ar::chip8::gpu::draw(ar_byte x, ar_byte y, ar_byte height, uint16_t register_i_value)
{
    // Whether any pixel has flipped to turned off, this will decide the return value and the value of register VF
    bool any_pixel_turned_off = false;

    // Height is defined by the caller
    for (ar_byte current_height = 0; current_height < height; current_height++)
    {
        // Start of pixel row
        ar_byte row_addr = _ram_link.read(register_i_value + current_height);

        // Every sprite has a width of 8
        for (ar_byte current_width = 0; current_width < 8; current_width++)
        {
            // Checks if this sprite pixel needs to be drawn
            if ((row_addr & (1 << (7 - current_width))) != 0)
            {
                // Add sprite coordinates to draw location to get this pixel location
                ar_byte flip_pixel_x_cord = x + current_width;
                ar_byte flip_pixel_y_cord = y + current_height;

                // Flip the pixel
                _frame_buffer.flip_pixel_channels(flip_pixel_x_cord, flip_pixel_y_cord);

                // Check if flipping the pixel turned it off
                auto& target_pixel = _frame_buffer.get_pixel_at(flip_pixel_x_cord, flip_pixel_y_cord);

                // Only need to check R because CHIP8 display is monochrome
                if (target_pixel.r == ar::chip8::PIXEL_TURNED_OFF_CHANNEL_VALUE)
                {
                    any_pixel_turned_off = true;
                }
            }
        }
    }

    // Set draw flag to update the screen
    set_draw_flag(true);

    // Return whether any pixel have flipped to turned off
    return static_cast<ar_byte>(any_pixel_turned_off);
}
