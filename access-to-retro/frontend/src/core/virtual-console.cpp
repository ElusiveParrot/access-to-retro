#include <SDL.h>
#include "util/logger.hpp"
#include "virtual-console.hpp"

/****************************************************************************************************
 virtual_console
****************************************************************************************************/

ar::core::virtual_console::virtual_console(const std::string& path) :
        _path(path),
        _library(path)
{
    // Defined by Access to Retro Developer Library
    std::function<ar::types::c_str()> get_name_fn;
    std::function<ar::types::c_str()> get_system_fn;
    std::function<ar::types::c_str()> get_author_fn;
    std::function<ar::types::c_str()> get_rom_ext_fn;

    std::function<unsigned()> get_frame_rate_fn;

    std::function<unsigned()> get_window_width_fn;
    std::function<unsigned()> get_window_height_fn;

    // Get functions pointers by querying the dynamic library using OS's API
    try
    {
        // Virtual Console Developer's defined functions (starting with '_')
        _define_fn  = _library.get_symbol<ar::types::err_code(*)()>("_ar_vc_define");
        _startup_fn = _library.get_symbol<ar::types::err_code(*)()>("_ar_vc_startup");
        _quit_fn    = _library.get_symbol<void(*)()>("_ar_vc_quit");

        // Access to Retro Developer Library thread functions
        _main_thread_fn   = _library.get_symbol<void(*)()>("_ar_vc_main_thread");
        _render_thread_fn = _library.get_symbol<void(*)()>("_ar_vc_render_thread");
        _input_thread_fn  = _library.get_symbol<void(*)()>("_ar_vc_input_thread");

        // Access to Retro Developer Library definition functions
        get_name_fn    = _library.get_symbol<ar::types::c_str(*)()>("ar_get_vc_name");
        get_system_fn  = _library.get_symbol<ar::types::c_str(*)()>("ar_get_vc_system");
        get_author_fn  = _library.get_symbol<ar::types::c_str(*)()>("ar_get_vc_author");
        get_rom_ext_fn = _library.get_symbol<ar::types::c_str(*)()>("ar_get_vc_rom_ext");

        get_frame_rate_fn = _library.get_symbol<unsigned(*)()>("ar_get_vc_frame_rate");

        get_window_width_fn  = _library.get_symbol<unsigned(*)()>("ar_get_vc_default_window_res_x");
        get_window_height_fn = _library.get_symbol<unsigned(*)()>("ar_get_vc_default_window_res_y");
    }
    catch (const ar::error::os_error& ex)
    {
        // Throw if ANY of the functions is not found, they are all REQUIRED
        throw ar::core::virtual_console_error("UNKNOWN",
                                              "Unable to create virtual console from file at: '" + path +
                                              "'\nDue to OS error: " + ex.get_logger_formatted_error());
    }

    ar::types::err_code define_res = _define_fn();
    if (define_res != 0)
    {
        throw ar::core::virtual_console_error("UNKNOWN",
                                              "Unable to create virtual console from file at: '" + path +
                                              "' as '_ar_vc_define' function has returned a non-zero error code (" +
                                              std::to_string(define_res) + ")");
    }

    // To get frame time (time each frame is on the screen) formula (1000 / frame rate) is used
    _frame_time = 1000.0 / get_frame_rate_fn();

    _default_window_width  = get_window_width_fn();
    _default_window_height = get_window_height_fn();

    ar::types::c_str console_name = get_name_fn();
    ar::types::c_str system_name  = get_system_fn();
    ar::types::c_str author_name  = get_author_fn();
    ar::types::c_str rom_ext      = get_rom_ext_fn();

    if (console_name == nullptr || system_name == nullptr || author_name == nullptr || rom_ext == nullptr)
    {
        std::string name = console_name == nullptr ? console_name : "UNKNOWN";

        throw ar::core::virtual_console_error(name,
                                              "Unable to create virtual console from file at: '" + path +
                                              "' as one of the meta-data getters has returned 'NULL', if you are the "
                                              "developer of this virtual console then make sure '_ar_vc_define' is "
                                              "working as it should");
    }

    _name          = console_name;
    _system        = system_name;
    _author        = author_name;
    _rom_extension = rom_ext;

    LOG_DEBUG("core.virtual_console", "Virtual console object with name '" + _name + "' created");
}

ar::core::virtual_console::virtual_console(virtual_console&& other) noexcept :
        _path(std::move(other._path)),
        _library(other._library),
        _name(std::move(other._name)),
        _system(std::move(other._system)),
        _author(std::move(other._author)),
        _rom_extension(std::move(other._rom_extension)),
        _frame_time(other._frame_time),
        _default_window_width(other._default_window_width),
        _default_window_height(other._default_window_height),
        _linked_binary(other._linked_binary),
        _define_fn(std::move(other._define_fn)),
        _startup_fn(std::move(other._startup_fn)),
        _quit_fn(std::move(other._quit_fn)),
        _run_threads(false),
        _main_thread(nullptr),
        _main_thread_fn(std::move(other._main_thread_fn)),
        _render_thread(nullptr),
        _render_thread_fn(std::move(other._render_thread_fn)),
        _input_thread(nullptr),
        _input_thread_fn(std::move(other._input_thread_fn))
{

}

ar::core::virtual_console::~virtual_console()
{
    LOG_DEBUG("core.virtual_console", "Virtual console object with name '" + _name + " destroyed");
}

void ar::core::virtual_console::prepare_for_startup(std::unique_ptr<ar::core::executable_binary>& binary)
{
    LOG_INFO("Preparing virtual console '" + _name + "' for startup..");

    _linked_binary = binary;

    LOG_INFO("Linked virtual console '" + _name + "' with binary '" + _linked_binary->get()->get_file_name() + "'");

    ar::types::err_code vc_startup_res = _startup_fn();
    if (vc_startup_res != 0)
    {
        throw ar::core::virtual_console_error(_name,
                                              "Unable to start virtual console, '_ar_vc_startup' returned a non-zero error code (" +
                                              std::to_string(vc_startup_res) + ")");
    }

    LOG_INFO("Virtual console '" + _name + "' is now ready to be started");
}

void ar::core::virtual_console::create_and_run_threads()
{
    _run_threads = true;

    _main_thread = std::make_unique<std::thread>(
            [&]
            {
                // Each time code in the loop runs it is refereed to as 'thread tick'
                while (_run_threads)
                {
                    // Timer value at the start of the thread tick
                    uint64_t start = SDL_GetPerformanceCounter();

                    _main_thread_fn();

                    // Timer value at the end of the thread tick
                    uint64_t end = SDL_GetPerformanceCounter();

                    // Calculate time elapsed from the beginning of the thread tick
                    float elapsed = (float) (end - start) / (float) SDL_GetPerformanceFrequency() * 1000.0f;

                    /*
                     * Delay next thread tick by frame time minus time taken to render.
                     *
                     * Frame time is the time that each frame has on the screen, elapsed is the time already elapsed so subtract it
                     */
                    SDL_Delay(static_cast<uint32_t>(floor(_frame_time - elapsed)));
                }
            });

    _render_thread = std::make_unique<std::thread>(
            [&]
            {
                // Each time code in the loop runs it is refereed to as 'thread tick'
                while (_run_threads)
                {
                    // Timer value at the start of the thread tick
                    uint64_t start = SDL_GetPerformanceCounter();

                    _render_thread_fn();

                    // Timer value at the end of the thread tick
                    uint64_t end = SDL_GetPerformanceCounter();

                    // Calculate time elapsed from the beginning of the thread tick
                    float elapsed = (float) (end - start) / (float) SDL_GetPerformanceFrequency() * 1000.0f;

                    /*
                     * Delay next thread tick by frame time minus time taken to render.
                     *
                     * Frame time is the time that each frame has on the screen, elapsed is the time already elapsed so subtract it
                     */
                    SDL_Delay(static_cast<uint32_t>(floor(_frame_time - elapsed)));
                }
            });

    _input_thread = std::make_unique<std::thread>(
            [&]
            {
                // Each time code in the loop runs it is refereed to as 'thread tick'
                while (_run_threads)
                {
                    // Timer value at the start of the thread tick
                    uint64_t start = SDL_GetPerformanceCounter();

                    _input_thread_fn();

                    // Timer value at the end of the thread tick
                    uint64_t end = SDL_GetPerformanceCounter();

                    // Calculate time elapsed from the beginning of the thread tick
                    float elapsed = (float) (end - start) / (float) SDL_GetPerformanceFrequency() * 1000.0f;

                    /*
                     * Delay next thread tick by frame time minus time taken to render.
                     *
                     * Frame time is the time that each frame has on the screen, elapsed is the time already elapsed so subtract it
                     */
                    SDL_Delay(static_cast<uint32_t>(floor(_frame_time - elapsed)));
                }
            });
}

void ar::core::virtual_console::quit_and_cleanup()
{
    _run_threads = false;

    // Join the threads
    _main_thread->join();
    _render_thread->join();
    _input_thread->join();

    // Call developer's defined quit function
    _quit_fn();

    LOG_INFO("Resources for virtual console '" + _name + "' deallocated");
}

std::string ar::core::virtual_console::get_path() const
{
    return _path;
}

std::string ar::core::virtual_console::get_name() const
{
    return _name;
}

std::string ar::core::virtual_console::get_system() const
{
    return _system;
}

std::string ar::core::virtual_console::get_author() const
{
    return _author;
}

std::string ar::core::virtual_console::get_rom_extension() const
{
    return _rom_extension;
}

unsigned ar::core::virtual_console::get_default_window_width() const
{
    return _default_window_width;
}

unsigned ar::core::virtual_console::get_default_window_height() const
{
    return _default_window_height;
}

/****************************************************************************************************
 virtual_console_error
****************************************************************************************************/

ar::core::virtual_console_error::virtual_console_error(std::string virtual_console_name, std::string error) :
        ar::base_exception("core.virtual_console"),
        _virtual_console_name(std::move(virtual_console_name)),
        _error(std::move(error))
{

}

std::string ar::core::virtual_console_error::get_vc_name() const
{
    return _virtual_console_name;
}

std::string ar::core::virtual_console_error::get_error() const
{
    return _error;
}

std::string ar::core::virtual_console_error::get_logger_formatted_error() const
{
    return "Error in virtual console: '" + get_vc_name() + "': " + get_error();
}
