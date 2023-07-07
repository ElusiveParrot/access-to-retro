#include <SDL.h>
#include "error/io-error.hpp"
#include "helpers/qt-helper.hpp"
#include "app.hpp"

ar::app::app(int argc, char** argv) :
        _qt_application(argc, argv),
        _splash_pixmap(ar::helpers::qt::resources::SPLASH_SCREEN),
        _splash(_splash_pixmap),
        _args(argv + 1, argv + argc)
{

}

ar::app::~app()
{
    SDL_Quit();
}

int ar::app::run()
{
    _splash.show();

    // Configure Libraries
    ar::helpers::qt::execute_splash_screen_event(_qt_application, _splash, [&]
    {
        if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
        {
            // TODO: Show window installed
            throw std::runtime_error("Unable to initialise SDL2 Library..");
        }
    }, "Configuring Libraries..");

    // Show the splash for extra 3 seconds
    ar::helpers::qt::execute_splash_screen_event(_qt_application, _splash, []
    {
        ar::helpers::qt::wait_with_event_processing(ar::util::time_duration(3));
    }, "Please wait..");

    // Splash screen finished so show main window now
    _main_window.show();
    _splash.finish(&_main_window);

    return _qt_application.exec(); // NOLINT(readability-static-accessed-through-instance)
}
