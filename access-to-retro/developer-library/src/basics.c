#include <access-to-retro-dev/access-to-retro-dev.h>

/****************************************************************************************************
 API global objects
****************************************************************************************************/

/// @brief Version of the access to retro library in standard Access to Retro versioning format
static struct ar_version g_lib_version = { .major = 1, .minor = 0, .patch = 0 };

/// @brief Version of the virtual console in standard Access to Retro versioning format
static struct ar_version g_vc_version;

/// @brief Name of the virtual console to be visible in the frontend
static const char* g_vc_name;

/// @brief Name of system that the virtual console emulates
static const char* g_vc_system;

/// @brief Name of the virtual console's author
static const char* g_vc_author;

/// @brief Extension that the ROM files will have, for ex: '*.nes'
static const char* g_vc_rom_ext;

/// @brief Frame rate that the virtual console is designed to run at
static unsigned g_vc_frame_rate;

/// @brief Default window width for this virtual console
static unsigned g_vc_default_window_x;

/// @brief Default window height for this virtual console
static unsigned g_vc_default_window_y;

/****************************************************************************************************
 API Implementation
****************************************************************************************************/

AR_API void ar_define(const char* name, const char* system, const char* author, const char* rom_ext,
                      struct ar_version version, unsigned frame_rate,
                      unsigned default_window_x, unsigned default_window_y)
{
    g_vc_name    = name != NULL ? name : "UNKNOWN";
    g_vc_system  = system != NULL ? system : "UNKNOWN";
    g_vc_author  = author != NULL ? author : "UNKNOWN";
    g_vc_rom_ext = rom_ext != NULL ? rom_ext : "*.rom";

    g_vc_version = version;

    g_vc_frame_rate = frame_rate;

    g_vc_default_window_x = default_window_x;
    g_vc_default_window_y = default_window_y;
}

AR_API void ar_init(void)
{
    ar_initialise_input_api();
}

AR_API void ar_quit(void)
{
    // Free graphical object
    ar_graphics_free_object();

    // Free executable object
    ar_free_executable();
}

AR_API struct ar_version ar_get_version(void)
{
    return g_lib_version;
}

AR_API const char* ar_get_vc_rom_ext(void)
{
    return g_vc_rom_ext;
}

AR_API const char* ar_get_vc_name(void)
{
    return g_vc_name;
}

AR_API const char* ar_get_vc_system(void)
{
    return g_vc_system;
}

AR_API const char* ar_get_vc_author(void)
{
    return g_vc_author;
}

AR_API unsigned ar_get_vc_frame_rate(void)
{
    return g_vc_frame_rate;
}

AR_API unsigned ar_get_vc_default_window_res_x(void)
{
    return g_vc_default_window_x;
}

AR_API unsigned ar_get_vc_default_window_res_y(void)
{
    return g_vc_default_window_y;
}
