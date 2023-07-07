/**
 * @file app/app.hpp
 */

#ifndef ACCESS_TO_RETRO_APP_HPP
#define ACCESS_TO_RETRO_APP_HPP

#include <QSplashScreen>
#include <QApplication>
#include <vector>
#include <string>
#include "gui/main-window.hpp"
#include "util/settings-manager.hpp"

/// @brief Root namespace of the project, contains important top-level classes and functions.
namespace ar
{
    /// @brief Application class containing entry point of the program, should be only created in main.
    class app
    {
    public:
        /**
         * @brief Default constructor
         * @param argc 'argc' from main function
         * @param argv 'argv' from main function
         */
        app(int argc, char** argv);
        ~app();

        /**
         * @brief Runs the application.
         * @return Status code returned by Qt.
         */
        int run();

    private:
        /// @brief Qt's main application singleton, MUST be constructed first before any other Qt's object
        QApplication _qt_application;

        /// @brief Image (map of pixels) used for the splash screen
        QPixmap _splash_pixmap;

        /// @brief Qt's splash screen, shown before the main window
        QSplashScreen _splash;

        /// @brief Main GUI screen shown after splash
        ar::gui::main_window _main_window;

        /// @brief Command line arguments in vector format
        std::vector<std::string> _args;

        /// @brief Pointer to global settings manager
        std::shared_ptr<ar::util::settings_manager> _settings_manager = nullptr;
    };
}

#endif //ACCESS_TO_RETRO_APP_HPP
