#include <QMainWindow>
#include <QFileDialog>
#include <QDir>
#include "gui/windows/input-settings-dialog.hpp"
#include "helpers/qt-helper.hpp"
#include "util/logger.hpp"
#include "first-time-setup-dialog.hpp"
#include "ui_first-time-setup-dialog.h"

ar::gui::first_time_setup_dialog::first_time_setup_dialog(QWidget* parent) :
        QDialog(parent),
        ui(new Ui::first_time_setup_dialog)
{
    ui->setupUi(this);

    _settings_manager = ar::util::settings_manager::get_global_manager();
}

ar::gui::first_time_setup_dialog::~first_time_setup_dialog()
{
    delete ui;
}

void ar::gui::first_time_setup_dialog::reject()
{
    // TODO: Change to not show main window
    ar::helpers::qt::show_error_message_box("Error!", "Please complete first time setup first.");
}

void ar::gui::first_time_setup_dialog::accept()
{
    // TODO: Set first_time_setup_done to true after validating settings

    // Validate both paths provided
    QString vcs_path   = ui->vc_path_textbox->text();
    QString games_path = ui->games_path_textbox->text();
    if (vcs_path.isEmpty() || games_path.isEmpty() ||
        !QDir(vcs_path).exists() || !QDir(games_path).exists())
    {
        // One or both paths invalid/empty
        ar::helpers::qt::show_error_message_box("Error!", "Invalid paths provided, make sure directories exist");

        // Don't allow exit if validation failed
        return;
    }
    else
    {
        // Both valid - set paths
        _settings_manager->set_setting("vc_path", vcs_path.toStdString());
        _settings_manager->set_setting("roms_path", games_path.toStdString());
    }

    // Set default source tab to systems
    _settings_manager->set_setting("default_source_tab", std::to_string(0));

    // If dev mode enabled then show every log message with rich verbosity
    _settings_manager->set_setting("logger_priority",
                                   std::to_string(
                                           (int) (ui->dev_mode_checkbox->isChecked() ?
                                                  ar::util::logger::priority::EVERYTHING_DEV :
                                                  ar::util::logger::priority::NOTHING)));

    _settings_manager->set_setting("logger_verbosity",
                                   std::to_string(
                                           (int) (ui->dev_mode_checkbox->isChecked() ?
                                                  ar::util::logger::verbosity::RICH :
                                                  ar::util::logger::verbosity::SIMPLE)));

    // TODO: set show_all_systems to true?

    // Validated, don't show this again
    _settings_manager->set_setting("first_time_setup_done", "true");

    // Open input settings, no need for RAII - Qt will auto free memory of this window
    auto input_settings = new ar::gui::input_settings_dialog(this);

    input_settings->exec();

    // Continue into main window
    QDialog::accept();
}

void ar::gui::first_time_setup_dialog::on_vc_path_browse_button_clicked()
{
    QString vcs_path = QFileDialog::getExistingDirectory(this, ("Select Folder"), QDir::homePath());
    if (!vcs_path.isEmpty())
    {
        ui->vc_path_textbox->setText(vcs_path);
    }
}

void ar::gui::first_time_setup_dialog::on_games_path_browse_button_clicked()
{
    QString games_path = QFileDialog::getExistingDirectory(this, ("Select Folder"), QDir::homePath());
    if (!games_path.isEmpty())
    {
        ui->games_path_textbox->setText(games_path);
    }
}
