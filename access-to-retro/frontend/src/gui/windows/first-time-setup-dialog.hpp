/**
 * @file gui/windows/first-time-setup-dialog.hpp
 */

#ifndef AR_GUI_FIRSTTIMESETUPDIALOG_HPP
#define AR_GUI_FIRSTTIMESETUPDIALOG_HPP

#include <QDialog>
#include "util/settings-manager.hpp"

namespace ar::gui
{
    namespace Ui
    {
        class first_time_setup_dialog;
    }

    /// @brief Dialog shown when user runs the app for the first time
    class first_time_setup_dialog : public QDialog
    {
    Q_OBJECT

    public:
        explicit first_time_setup_dialog(QWidget* parent = nullptr);
        ~first_time_setup_dialog() override;

        // ****************** Qt's Slot Methods ******************
    private slots:
        /// @brief Override exit behaviour
        void reject() override;

        /// @brief Override 'ok' button behaviour
        void accept() override;

        /// @brief Runs when browse button for virtual consoles path is clicked
        void on_vc_path_browse_button_clicked();

        /// @brief Runs when browse button for games path is clicked
        void on_games_path_browse_button_clicked();

    private:
        /// @brief Global settings manager object fetched as a pointer
        std::shared_ptr<ar::util::settings_manager> _settings_manager;

        /// @brief Qt's autogenerated Ui
        Ui::first_time_setup_dialog* ui;
    };
}

#endif // AR_GUI_FIRSTTIMESETUPDIALOG_HPP
