/**
 * @file helpers/qt-helper.hpp
 */

#ifndef ACCESS_TO_RETRO_QT_HELPER_HPP
#define ACCESS_TO_RETRO_QT_HELPER_HPP

#include <QSplashScreen>
#include <QApplication>
#include <QTime>
#include "helpers/types.hpp"
#include "util/time.hpp"

/// @brief Helper classes and function with various functionality
namespace ar::helpers
{
    // Exists for the doc comment above, made it this way to avoid nesting in below namespace
}

namespace ar::helpers::qt
{
    /**
     * @brief Helper function to create QVariant objects with custom class type
     * @tparam T Custom class type
     * @param type Custom class's QMetaType object
     * @param value Value of created variant
     * @return Created QVariant
     */
    template<typename T>
    static QVariant create_variant(QMetaType type, T value)
    {
        QVariant variant = QVariant(type);
        variant.setValue(value);

        return variant;
    }

    /**
     * @brief Similar to OS's wait but affects only Qt's thread.
     * @param duration Time to 'wait'
     */
    void wait_with_event_processing(ar::util::time_duration duration);

    /**
     * @brief Used to execute an action while the splash screen is visible.
     * @param app Main Qt application object
     * @param action Lambda defining the action that should happen.
     */
    void execute_splash_screen_event(const QApplication& app, QSplashScreen& splash,
                                     const std::function<void()>& action, const QString& waiting_message);

    /**
     * @brief Shows a message box with 'critical' severity
     * @param title Title at the top of the message box
     * @param message Message in the middle of the message box
     * @param detailed_text Text below 'message'
     * @param size Size of the message box
     * @param parent Parent window of the message box
     */
    void show_error_message_box(const QString& title, const QString& message, const QString& detailed_text = "",
                                QSize size = QSize(500, 200), QWidget* parent = nullptr);


    /// @brief Abstraction for a path for Qt resource in .qrc file
    typedef ar::types::c_str qt_resource_path;

    /// @brief Contains paths to the resources inside .qrc file for Qt
    namespace resources
    {
        /// @brief Image used for the splash screen shown before the main window
        constexpr qt_resource_path SPLASH_SCREEN = ":/splash.png";

        /// @brief Icon for a list of games
        constexpr qt_resource_path GAME_ICON = ":/game_icon.png";

        /**
         * @brief Checks if resource exists inside the .qrc file
         * @param relative_path_to_root Path to the resource excluding ':/'
         * @return Whether the file exists or not
         */
        bool check_if_resource_exists(const qt_resource_path& relative_path_to_root);
    }
}

#endif //ACCESS_TO_RETRO_QT_HELPER_HPP
