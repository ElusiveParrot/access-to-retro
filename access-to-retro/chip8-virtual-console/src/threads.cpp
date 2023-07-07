/**
 * @file threads.cpp
 */

#include <cmath>
#include <access-to-retro-dev/access-to-retro-dev.h>
#include "emulator/emulator.hpp"

/**
 * @brief Main thread function
 * @details Runs when the virtual console is started, should contain an infinite loop (it will stop automatically
 *          when the user closes the window), while the developer is free to use this thread as he likes (even to create
 *          more threads) it is recommended to use this thread for main fetch execute decode cycle and inter-component
 *          communication (for ex: CPU <-> GPU).
 * @remarks This function will loop in a loop in a thread every frame time (1000 / frame rate)
 *          (for example: every 16.6ms if the frame rate is 60)
 * @remark This function NEEDS to be defined for the Access to Retro library.
 */
AR_DEFINE_REQUIRED_FN(AR_THREAD_MAIN_FN)
{
    ar::chip8::cpu& cpu = ar::chip8::emulator::get_global_emulator()->access_cpu();

    /*
     * To calculate how many instructions need to be executed per frame it can be calculated using
     * formula: CLOCK_SPEED / FRAME_RATE
     *
     * Clock speed is how many instructions are executed per second
     * Frame rate is how many frames are shown per second (how many times this function gets called too)
     *
     * So, for framerate of 60 this function gets called 60 times per second and for clock speed of 600hz
     * 600 instructions needs to be executed per second therefore 600 / 60 = 10;
     */
    for (auto i = 0; i < std::floor(ar::chip8::CLOCK_SPEED / ar::chip8::FRAME_RATE); i++)
    {
        // Execute one instruction (one cpu tick)
        cpu.tick();
    }

    // Timers should tick at constant 60hz and not 600hz that cpu runs on so tick timers here and not in the loop
    cpu.tick_timers();
}

/**
 * @brief Render thread function
 * @details Runs when the virtual console is started, should contain an infinite loop (it will stop automatically
 *          when the user closes the window), while the developer is free to use this thread as he likes (even to create
 *          more threads) it is recommended to use this thread for rendering, whether that's OpenGL, SDL2 or direct
 *          frame buffer access.
 * @remarks This function will loop in a loop in a thread every frame time (1000 / frame rate)
 *          (for example: every 16.6ms if the frame rate is 60)
 * @remark This function NEEDS to be defined for the Access to Retro library.
 */
AR_DEFINE_REQUIRED_FN(AR_THREAD_RENDER_FN)
{
    ar::chip8::gpu& gpu = ar::chip8::emulator::get_global_emulator()->access_gpu();

    if (gpu.get_draw_flag())
    {
        // If draw flag is set then draw to the screen
        gpu.render();

        // Let emulator know rendering happened (clear CPU's draw flag)
        gpu.set_draw_flag(false);
    }
}

/**
 * @brief Input thread function
 * @details Runs when the virtual console is started, should contain an infinite loop (it will stop automatically
 *          when the user closes the window), while the developer is free to use this thread as he likes (even to create
 *          more threads) it is recommended to use this thread for input handling
 * @warning SDL should NOT be used to handle input by the developer, only access to retro library helpers should be used
 *           this is due to the fact that SDL does not allow to poll events outside of the thread where it was
 *           initialised and doing so will crash the application.
 * @remarks This function will loop in a loop in a thread every frame time (1000 / frame rate)
 *          (for example: every 16.6ms if the frame rate is 60)
 * @remark This function NEEDS to be defined for the Access to Retro library.
 */
AR_DEFINE_REQUIRED_FN(AR_THREAD_INPUT_FN)
{
    ar::chip8::controller& controller = ar::chip8::emulator::get_global_emulator()->access_controller();


    controller.set_key_status(ar::chip8::key::key_4, ar_get_unified_controller_key_status
            (ar_unified_controller_left_analog_left));

    controller.set_key_status(ar::chip8::key::key_5, ar_get_unified_controller_key_status
            (ar_unified_controller_right_trigger));

    controller.set_key_status(ar::chip8::key::key_6, ar_get_unified_controller_key_status
            (ar_unified_controller_left_analog_right));
}
