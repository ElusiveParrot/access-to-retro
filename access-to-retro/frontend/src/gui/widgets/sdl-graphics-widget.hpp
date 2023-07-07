/**
 * @file gui/widgets/sdl-graphics-widget.hpp
 */

#ifndef AR_GUI_SDLGRAPHICSWIDGET_HPP
#define AR_GUI_SDLGRAPHICSWIDGET_HPP

#include <access-to-retro-dev/access-to-retro-dev.h>
#include <QKeyEvent>
#include <QWidget>
#include <QTimer>
#include <SDL.h>
#include "core/executable-binary.hpp"
#include "core/virtual-console.hpp"
#include "core/input-handler.hpp"

namespace ar::gui
{
    /// @brief Widget used to allow SDL2 to render onto a Qt
    class sdl_graphics_widget : public QWidget
    {
    public:
        /**
         * @brief Default constructor
         * @param virtual_console Virtual console that will output to this widget
         * @param game Game that will be played in this widget
         * @param parent Parent window
         * @throws Exceptions:
         *  - std::runtime_error: Unable to initialise SDL objects
         *  - ar::core::virtual_console_error: Virtual console threw an error
         */
        sdl_graphics_widget(std::unique_ptr<ar::core::virtual_console>& virtual_console,
                            std::unique_ptr<ar::core::executable_binary>& game,
                            QWidget* parent = nullptr);
        ~sdl_graphics_widget() override;

    private:
        /**
         * @brief Defines framerate for the widget
         * @todo Enable developer to set his own (low priority)
         */
        static constexpr int FRAME_RATE = 60;

        /// @brief Virtual console that will output to this widget
        std::unique_ptr<ar::core::virtual_console>& _virtual_console;

        /// @brief Game that will be played in this widget
        std::unique_ptr<ar::core::executable_binary>& _game;

        /// @brief Object handling input and directing it to the virtual console
        ar::core::input_handler _input_handler;

        /// @brief Prepares virtual console and game for launch
        void prepare_for_game_launch();

        /**
         * @brief Prepare graphical part of Access to Retro library inside the virtual console
         * @throws Exceptions:
         *  - ar::core::virtual_console_error: Invalid graphical mode (or ar_graphics_mode_none) set by the virtual
         *                                     console
         */
        void prepare_graphics();

        /// @brief Dummy function silencing Qt's paint engine bug error
        QPaintEngine* paintEngine() const override;

        /// @brief Timer used to time when to handle user input
        QTimer _frame_timer;

        // ****************** SDL's objects ******************

        /// @brief Emulated SDL window created from Qt's openGL widget
        SDL_Window* _sdl_window = nullptr;

        /// @brief Basic SDL renderer
        SDL_Renderer* _sdl_renderer = nullptr;

        /**
         * @brief Initialise objects from sdl library
         * @throws Exceptions:
         *  - std::runtime_error: Unable to initialise SDL objects
         */
        void initialise_sdl_objects();

        /// @brief Destroy objects from sdl library
        void destroy_sdl_objects();

        /**
         * @brief Handles resize event on SDL side - does nothing for now
         * @param w New width
         * @param h New height
         */
        void on_resize(int w, int h);

        // ****************** Qt's event overwrites ******************

        /// @brief Qt's widget resize event handler
        void resizeEvent(QResizeEvent* event) override;

        /// @brief Overrides Qt's Key press event for input handling
        void keyPressEvent(QKeyEvent* event) override;

        /// @brief Overrides Qt's Key release event for input handling
        void keyReleaseEvent(QKeyEvent* event) override;

        // ****************** Access to Retro Developer Library objects ******************

        /// @brief Access to Retro's graphics method, set using 'ar_graphics_set_method' by virtual console
        ar_graphics_method _graphics_method = ar_graphics_method_none;

        /**
         * @brief Frame buffer object from the Access to Retro library created by the developer if graphics mode is SDL
         * @remark Freed by the library automatically
         */
        ar_frame_buffer* _frame_buffer = nullptr;

    };
}

#endif // AR_GUI_SDLGRAPHICSWIDGET_HPP
