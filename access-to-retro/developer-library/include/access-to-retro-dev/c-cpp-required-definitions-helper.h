/**
 * @file c-cpp-required-definitions-helper.h
 * This should ONLY be used when using C/C++ for your virtual console
 */

/** @defgroup group_def Required Definitions Helper
 *  Contains helper macros for usage in C/C++ virtual console projects
 *  @{
 */

#ifndef ACCESS_TO_RETRO_C_CPP_REQUIRED_DEFINITIONS_HELPER_H
#define ACCESS_TO_RETRO_C_CPP_REQUIRED_DEFINITIONS_HELPER_H

#include "basics.h"

/**
 * @brief Defines a function required
 * @param signature Signature of the function, most likely another macro - it's discouraged to use manual signatures
 */
#define AR_DEFINE_REQUIRED_FN(signature) AR_API signature

/************************ DEFINITION FUNCTIONS ************************/

/**
 * @brief Defines virtual console's metadata for the frontend to use.
 * @details Runs when the virtual console is loaded by the frontend (not when it's started).
 * @remark This function NEEDS to be defined for the Access to Retro library.
 * @return Error code:
 *  - 0: No error,
 *  - Anything else: In this virtual console it's result from ar_define.
 */
#define AR_DEFINE_FN ar_err_code _ar_vc_define(void)

/**
 * @brief Configures the virtual console and the developer library.
 * @details Runs when the virtual console is started, should be used to configure virtual console's internal state and
 *          the library.
 * @remark This function NEEDS to be defined for the Access to Retro library.
 * @return Error code:
 *  - 0: No error
 *  - Anything else: In this virtual console it's the result from ar_init.
 */
#define AR_STARTUP_FN ar_err_code _ar_vc_startup(void)

/**
 * @brief Clean up resources of the virtual console and library.
 * @details Runs when the virtual console is closed, should be used to deallocate memory and resources.
 * @remark This function NEEDS to be defined for the Access to Retro library.
 */
#define AR_QUIT_FN void _ar_vc_quit(void)

/************************ THREAD FUNCTIONS ************************/

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
#define AR_THREAD_MAIN_FN void _ar_vc_main_thread(void)

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
#define AR_THREAD_RENDER_FN void _ar_vc_render_thread(void)

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
#define AR_THREAD_INPUT_FN void _ar_vc_input_thread(void)


#endif //ACCESS_TO_RETRO_C_CPP_REQUIRED_DEFINITIONS_HELPER_H

/** @} */ // end of group
