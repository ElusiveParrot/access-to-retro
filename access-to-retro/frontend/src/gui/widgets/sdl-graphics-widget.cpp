#include <cmath>
#include "helpers/qt-helper.hpp"
#include "util/logger.hpp"
#include "sdl-graphics-widget.hpp"

ar::gui::sdl_graphics_widget::sdl_graphics_widget(std::unique_ptr<ar::core::virtual_console>& virtual_console,
                                                  std::unique_ptr<ar::core::executable_binary>& game,
                                                  QWidget* parent) :
        QWidget(parent),
        _virtual_console(virtual_console),
        _game(game),
        _input_handler(_virtual_console)
{
    // Set Qt's window attributes for painting
    setAttribute(Qt::WA_OpaquePaintEvent);
    setAttribute(Qt::WA_NoSystemBackground);
    setFocusPolicy(Qt::StrongFocus);
    setAttribute(Qt::WA_PaintOnScreen);

    initialise_sdl_objects();

    prepare_graphics();

    prepare_for_game_launch();
}

ar::gui::sdl_graphics_widget::~sdl_graphics_widget()
{
    _virtual_console->quit_and_cleanup();

    _game->unload_file();

    destroy_sdl_objects();
}

void ar::gui::sdl_graphics_widget::initialise_sdl_objects()
{
    _sdl_window = SDL_CreateWindowFrom((void*) winId());
    if (_sdl_window == nullptr)
    {
        throw std::runtime_error("Unable to initialise SDL2 window");
    }

    auto renderer_flags =
#if defined(_WIN32)
   SDL_RENDERER_SOFTWARE;
#else
    SDL_RENDERER_ACCELERATED;
#endif

    _sdl_renderer = SDL_CreateRenderer(_sdl_window, -1, renderer_flags);
    if (_sdl_renderer == nullptr)
    {
        throw std::runtime_error("Unable to initialise SDL2 renderer");
    }

    // Clear the screen
    SDL_SetRenderDrawColor(_sdl_renderer, 0, 0, 0, 255);
    SDL_RenderPresent(_sdl_renderer);

    LOG_DEBUG("gui.sdl_graphics_widget", "SDL objects initialised");
}

void ar::gui::sdl_graphics_widget::destroy_sdl_objects()
{
    SDL_DestroyRenderer(_sdl_renderer);
    SDL_DestroyWindow(_sdl_window);

    LOG_DEBUG("gui.sdl_graphics_widget", "SDL objects destroyed");
}

void ar::gui::sdl_graphics_widget::prepare_for_game_launch()
{
    try
    {
        _game->load_to_memory();

        // Set the binary file in access to retro library
        auto set_binary_fn =
                _virtual_console->get_symbol_from_library<void(*)(const ar_byte*, size_t)>("ar_create_executable");
        set_binary_fn(_game->get_file_raw_bytes(), _game->get_file_size());

        LOG_DEBUG("gui.sdl_graphics_widget", "Retro to access library received executable object");

        _virtual_console->prepare_for_startup(_game);
        _virtual_console->create_and_run_threads();

        LOG_DEBUG("gui.sdl_graphics_widget", "Virtual Console threads started");
    }
    catch (const ar::error::io_error&)
    {
        ar::helpers::qt::show_error_message_box("Error!", "Unable to load game to memory!",
                                                "Please check that file still exists");

        close();

        return;
    }
    catch (const ar::core::virtual_console_error& ex)
    {
        ar::helpers::qt::show_error_message_box("Error!", "Unable to start virtual console!",
                                                QString::fromStdString(ex.get_logger_formatted_error()));

        close();

        return;
    }
}

void ar::gui::sdl_graphics_widget::prepare_graphics()
{
    // Get graphical method from the library
    auto get_method_fn =
                 _virtual_console->get_symbol_from_library<ar_graphics_method(*)()>("ar_graphics_get_method");

    _graphics_method = get_method_fn();

    switch (_graphics_method)
    {
        case ar_graphics_method_frame_buffer:
        {
            LOG_DEBUG("gui.virtual_console", "Virtual Console graphics mode found to be: FRAME_BUFFER");

            // Get the frame buffer getter fn
            auto get_frame_buffer_obj =
                         _virtual_console->get_symbol_from_library<ar::types::c_generic_memory_ptr(*)()>
                                                 ("ar_graphics_get_frame_buffer");

            // Get the frame buffer and cast the generic memory pointer to the frame buffer
            _frame_buffer = (ar_frame_buffer*) get_frame_buffer_obj();

            // TODO: Create thread that draws the buffer continuously

            break;
        }

        case ar_graphics_method_open_gl_context:
        {
            LOG_DEBUG("gui.virtual_console", "Virtual Console graphics mode found to be: OPEN_GL");

            // Set OpenGL context
            auto set_gl_context_fn = _virtual_console->get_symbol_from_library<void(*)(open_gl_context)>
                ("ar_graphics_set_gl_context");

            set_gl_context_fn(SDL_GL_CreateContext(_sdl_window));
        }
            break;

        case ar_graphics_method_sdl:
        {
            LOG_DEBUG("gui.virtual_console", "Virtual Console graphics mode found to be: SDL2");

            // Get library's setter
            auto set_sdl_objects_fn =
            _virtual_console->get_symbol_from_library<void(*)(SDL_Window* , SDL_Renderer*)>("ar_graphics_set_sdl_objects");

            // Provide frontend's sdl objects
            set_sdl_objects_fn(_sdl_window, _sdl_renderer);

            break;
        }

        case ar_graphics_method_none:
        default:
            throw ar::core::virtual_console_error(_virtual_console->get_name(),
                                                  "Invalid graphical mode set inside the virtual console (" +
                                                  std::to_string(_graphics_method) + ")");
    }
}

void ar::gui::sdl_graphics_widget::keyPressEvent(QKeyEvent* event)
{
    _input_handler.key_press(QKeySequence(event->key()).toString().toStdString());
}

void ar::gui::sdl_graphics_widget::keyReleaseEvent(QKeyEvent* event)
{
    _input_handler.key_release(QKeySequence(event->key()).toString().toStdString());
}

void ar::gui::sdl_graphics_widget::on_resize([[maybe_unused]] int w, [[maybe_unused]] int h)
{
    // TODO: Do nothing for now
}

void ar::gui::sdl_graphics_widget::resizeEvent(QResizeEvent*)
{
    // Get Qt's new sizes
    int w = static_cast<int>(QWidget::width());
    int h = static_cast<int>(QWidget::height());

    // Handle in SDL2
    on_resize(w, h);

    LOG_DEBUG("gui.sdl_graphics_widget", "Window resided to new dimensions ("
                                         + std::to_string(w) + "," + std::to_string(h) + ")");
}

QPaintEngine* ar::gui::sdl_graphics_widget::paintEngine() const
{
    return reinterpret_cast<QPaintEngine*>(0);
}
