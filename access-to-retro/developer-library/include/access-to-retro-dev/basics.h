/**
 * @file basics.h
 */

/** @defgroup group_basics Basics
 *  Basic functions to define/start/quit the virtual console
 *  @{
 */

#ifndef ACCESS_TO_RETRO_BASICS_H
#define ACCESS_TO_RETRO_BASICS_H

#include <stdint.h>

#ifdef __cplusplus

/**
 * @def AR_API
 * @brief Macro that should be used before Retro Access API functions to force C linkage and disable symbol mangling
 */
#ifdef _WIN32 // Windows version
#define AR_API extern "C" __declspec(dllexport)
#else // POSIX version
#define AR_API extern "C"
#endif

#else

#ifdef _WIN32 // Windows version
#define AR_API __declspec(dllexport)
#else // POSIX version
#define AR_API
#endif

#endif

/// @brief Type representing error code in Access Retro library.
typedef uint32_t ar_err_code;

/// @brief Struct representing versioning system used by Access Retro.
struct ar_version
{
    /**
     * @brief X.0.0
     * @details Part of the version updated on major changes/overhauls.
     */
    uint16_t major;

    /**
     * @brief 0.X.0
     * @details Part of the version updated on minor changes or small feature updates.
     */
    uint16_t minor;

    /**
     * @brief 0.0.X
     * @details Part of the version updated on very small changes.
     */
    uint16_t patch;
};

/**
 * @brief Define the virtual console for Access to Retro library
 * @details Should always be called in '_ar_vc_define' function that needs to be defined by the virtual console
 * @warning If NULL is provided as any of the string arguments then a default value of UNKNOWN will be used unless
 *          it's the 'rom_ext' in which case '*.rom' will be used. It is not advisable to use NULL for any of the
 *          values.
 * @param name Name of the virtual console
 * @param system System that is emulated
 * @param author Author of the virtual console
 * @param rom_ext Extension of the ROM files, for example: '*.nes'
 * @param version Current version of the virtual console
 * @return Error codes:
 *  - 0: No error
 */
AR_API void
ar_define(const char* name, const char* system, const char* author, const char* rom_ext, struct ar_version version,
          unsigned frame_rate, unsigned default_window_width, unsigned default_window_height);

/**
 * @brief Initialise Access Retro dev library
 * @details Should ALWAYS be called in '_ar_vc_startup' function that needs to be defined by the virtual console
 * @return Error codes:
 *  - 0: No error
 */
AR_API void ar_init(void);

/**
 * @brief Clean up everything related to Access Retro library
 * @details Should always be called in '_ar_vc_quit' function that needs to be defined by the virtual console
 * @warning Possibility of memory leak if this function is not be called in '_ar_vc_quit'
 */
AR_API void ar_quit(void);

/**
 * @brief Get the version of the Access to Retro library itself
 * @return Structure containing the version numbers
 */
AR_API struct ar_version ar_get_version(void);

/**
 * @brief Get the virtual console's ROM's extension
 * @return Virtual console's ROM's extension in format '*.extension'
 */
AR_API const char* ar_get_vc_rom_ext(void);

/**
 * @brief Get the virtual console's name
 * @return Virtual console's name
 */
AR_API const char* ar_get_vc_name(void);

/**
 * @brief Get the virtual console's system name
 * @return Virtual console's system name, if configured correctly it should follow Access to Retro standard
 */
AR_API const char* ar_get_vc_system(void);

/**
 * @brief Get the virtual console's author name
 * @return Virtual console's author name
 */
AR_API const char* ar_get_vc_author(void);

/**
 * @brief Get the virtual console's frame rate
 * @return Virtual console's frame rate
 */
AR_API unsigned ar_get_vc_frame_rate(void);

/**
 * @brief @brief Get the virtual console's default window width
 * @return Virtual console's default resolution X component
 */
AR_API unsigned ar_get_vc_default_window_res_x(void);

/**
 * @brief @brief Get the virtual console's default window height
 * @return Virtual console's default resolution Y component
 */
AR_API unsigned ar_get_vc_default_window_res_y(void);

#endif

/** @} */ // end of group
