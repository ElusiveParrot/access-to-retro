/**
 * @file config.cpp
 */

#include <access-to-retro-dev/access-to-retro-dev.h>
#include "emulator/emulator.hpp"

// Define basic information about the emulator for the Access to Retro library
constexpr const char* NAME    = "access-to-retro-chip8";
constexpr const char* SYSTEM  = "CHIP8";
constexpr const char* AUTHOR  = "Daniel Wenda";
constexpr const char* ROM_EXT = "*.ch8";

constexpr unsigned FRAME_RATE = 60;

// Use CHIP8's resolution multiplied by 10 for default resolution (integer scaling) to avoid stretching
constexpr unsigned DEFAULT_WINDOW_WIDTH  = ar::chip8::SCREEN_RESOLUTION_X * 10;
constexpr unsigned DEFAULT_WINDOW_HEIGHT = ar::chip8::SCREEN_RESOLUTION_Y * 10;

constexpr ar_version VERSION { .major = 1, .minor = 0, .patch = 0 };

/**
 * @brief Defines virtual console's metadata for the frontend to use.
 * @details Runs when the virtual console is loaded by the frontend (not when it's started).
 * @remark Graphics mode should also be configured here, using 'ar_graphics_set_method'
 * @remark This function NEEDS to be defined for the Access to Retro library.
 * @return Error code:
 *  - 0: No error,
 *  - Anything else: In this virtual console it's result from ar_define.
 */
AR_DEFINE_REQUIRED_FN(AR_DEFINE_FN)
{
    ar_define(NAME, SYSTEM, AUTHOR, ROM_EXT, VERSION,
              FRAME_RATE, DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT);

    // Set graphics method to SDL
    ar_graphics_set_method(ar_graphics_method_sdl);

    return 0;
}

/**
 * @brief Configures the virtual console and the developer library.
 * @details Runs when the virtual console is started, should be used to configure virtual console's internal state and
 *          the library.
 * @remark This function NEEDS to be defined for the Access to Retro library.
 * @return Error code:
 *  - 0: No error
 *  - Anything else: In this virtual console it's the result from ar_init.
 */
AR_DEFINE_REQUIRED_FN(AR_STARTUP_FN)
{
    ar_init();

    ar::chip8::emulator::create_global_emulator();

    ar::chip8::emulator::get_global_emulator()->access_ram().load_binary(ar_get_executable());

    return 0;
}

/**
 * @brief Clean up resources of the virtual console and library.
 * @details Runs when the virtual console is closed, should be used to deallocate memory and resources.
 * @remark This function NEEDS to be defined for the Access to Retro library.
 */
AR_DEFINE_REQUIRED_FN(AR_QUIT_FN)
{
    ar_quit();
}
