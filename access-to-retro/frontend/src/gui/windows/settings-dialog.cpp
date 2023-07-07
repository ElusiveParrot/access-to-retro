#include <QFileDialog>
#include <QCheckBox>
#include <utility>
#include "gui/main-window.hpp"
#include "helpers/qt-helper.hpp"
#include "util/logger.hpp"
#include "settings-dialog.hpp"
#include "ui_settings-dialog.h"

ar::gui::settings_dialog::settings_dialog(QWidget* parent) :
        QDialog(parent),
        ui(new Ui::settings_dialog)
{
    _settings_manager = ar::util::settings_manager::get_global_manager();

    ui->setupUi(this);

    // Populate the Ui
    populate_combo_boxes();
    populate_paths();
    populate_checkboxes();

    // Ui populated - accept events
    _ui_created = true;
}

ar::gui::settings_dialog::~settings_dialog()
{
    delete ui;
}

void ar::gui::settings_dialog::on_priority_combo_box_currentTextChanged(const QString&)
{
    // Ignore changes before the UI is populated
    if (!_ui_created)
    {
        return;
    }

    QVariant selected_data = ui->priority_combo_box->currentData();
    auto     new_priority  = selected_data.value<ar::util::logger::priority>();

    // Update logger's minimum priority and app settings file
    ar::util::logger::set_minimum_priority(new_priority);
    _settings_manager->set_setting("logger_priority", std::to_string((int) new_priority));
}

void ar::gui::settings_dialog::on_verbosity_combo_box_currentTextChanged(const QString&)
{
    // Ignore changes before the UI is populated
    if (!_ui_created)
    {
        return;
    }

    QVariant selected_data = ui->verbosity_combo_box->currentData();
    auto     new_verbosity = selected_data.value<ar::util::logger::verbosity>();

    // Update logger's verbosity and app settings file
    ar::util::logger::set_verbosity(new_verbosity);
    _settings_manager->set_setting("logger_verbosity", std::to_string((int) new_verbosity));
}

void ar::gui::settings_dialog::on_vc_path_browse_button_clicked()
{
    QString new_vc_path = QFileDialog::getExistingDirectory(this, ("Select Folder"), QDir::homePath());
    if (!new_vc_path.isEmpty())
    {
        // Path is not empty so directory was chosen - change it in settings file
        _settings_manager->set_setting("vc_path", new_vc_path.toStdString());
        ui->vc_path_line_edit->setText(new_vc_path);
    }
}

void ar::gui::settings_dialog::on_rendering_combo_box_currentIndexChanged(int index)
{
    if (!_ui_created)
    {
        return;
    }

    _settings_manager->set_setting("rendering", std::to_string(index));
}

void ar::gui::settings_dialog::on_rom_path_browse_button_clicked()
{
    QString new_roms_path = QFileDialog::getExistingDirectory(this, ("Select Folder"), QDir::homePath());
    if (!new_roms_path.isEmpty())
    {
        // Path is not empty so directory was chosen - change it in settings file
        _settings_manager->set_setting("roms_path", new_roms_path.toStdString());
        ui->rom_path_line_edit->setText(new_roms_path);
    }
}

void ar::gui::settings_dialog::on_bottom_button_box_clicked(QAbstractButton* clicked_button)
{
    QDialogButtonBox::ButtonRole button_clicked_role = ui->bottom_button_box->buttonRole(clicked_button);

    switch (button_clicked_role)
    {
        case QDialogButtonBox::ButtonRole::ResetRole:
            // TODO: Reset settings
            break;

        case QDialogButtonBox::ButtonRole::AcceptRole:
            this->close();
            break;

        default:
            LOG_DEBUG("gui.settings_window", "Unhandled button box button clicked");
            break;
    }
}

void ar::gui::settings_dialog::on_default_source_tab_combo_box_currentIndexChanged(int index)
{
    if (!_ui_created)
    {
        return;
    }

    _settings_manager->set_setting("default_source_tab", std::to_string(index));
}

void ar::gui::settings_dialog::on_show_all_systems_checkbox_stateChanged(int arg)
{
    std::string value = Qt::CheckState(arg) == Qt::CheckState::Checked ? "true" : "false";
    _settings_manager->set_setting("show_all_systems", value);
}

void ar::gui::settings_dialog::populate_combo_boxes()
{
    // ****************** Developer : Logger Priority ******************
    QMetaType priority_type = QMetaType::fromType<ar::util::logger::priority>();

    ui->priority_combo_box->addItem("Everything (Debug)",
                                    ar::helpers::qt::create_variant(priority_type,
                                                                    ar::util::logger::priority::EVERYTHING_DEV));
    ui->priority_combo_box->addItem("Everything",
                                    ar::helpers::qt::create_variant(priority_type,
                                                                    ar::util::logger::priority::EVERYTHING));
    ui->priority_combo_box->addItem("Warning",
                                    ar::helpers::qt::create_variant(priority_type,
                                                                    ar::util::logger::priority::WARNING));
    ui->priority_combo_box->addItem("Error",
                                    ar::helpers::qt::create_variant(priority_type,
                                                                    ar::util::logger::priority::APP_ERROR));
    ui->priority_combo_box->addItem("Critical",
                                    ar::helpers::qt::create_variant(priority_type,
                                                                    ar::util::logger::priority::CRITICAL));
    ui->priority_combo_box->addItem("Nothing",
                                    ar::helpers::qt::create_variant(priority_type,
                                                                    ar::util::logger::priority::NOTHING));

    // Set based on settings
    auto logger_priority_setting_res = _settings_manager->get_setting("logger_priority");
    if (logger_priority_setting_res != std::nullopt)
    {
        ui->priority_combo_box->setCurrentIndex(std::stoi(logger_priority_setting_res.value()));
    }
    else
    {
        // Setting not found so set to CRITICAL
        _settings_manager->set_setting("logger_priority", std::to_string((int) ar::util::logger::priority::CRITICAL));
        ui->priority_combo_box->setCurrentIndex((int) ar::util::logger::priority::CRITICAL);
    }

    // ****************** Developer : Logger Verbosity ******************
    QMetaType verbosity_type = QMetaType::fromType<ar::util::logger::verbosity>();

    ui->verbosity_combo_box->addItem("Simple",
                                     ar::helpers::qt::create_variant(verbosity_type,
                                                                     ar::util::logger::verbosity::SIMPLE));
    ui->verbosity_combo_box->addItem("Rich",
                                     ar::helpers::qt::create_variant(verbosity_type,
                                                                     ar::util::logger::verbosity::RICH));
    ui->verbosity_combo_box->addItem("Debug",
                                     ar::helpers::qt::create_variant(verbosity_type,
                                                                     ar::util::logger::verbosity::DEBUG));

    // Set based on settings
    auto logger_verbosity_setting_res = _settings_manager->get_setting("logger_verbosity");
    if (logger_verbosity_setting_res != std::nullopt)
    {
        ui->verbosity_combo_box->setCurrentIndex(std::stoi(logger_verbosity_setting_res.value()));
    }
    else
    {
        // Setting not found so set to SIMPLE
        _settings_manager->set_setting("logger_verbosity", std::to_string((int) ar::util::logger::verbosity::SIMPLE));
        ui->verbosity_combo_box->setCurrentIndex((int) ar::util::logger::verbosity::SIMPLE);
    }

    // ****************** General : Main Window Source Tab ******************
    // Set based on settings if it exists
    auto default_source_tab_setting_res = _settings_manager->get_setting("default_source_tab");
    if (default_source_tab_setting_res != std::nullopt)
    {
        ui->default_source_tab_combo_box->setCurrentIndex(std::stoi(default_source_tab_setting_res.value()));
    }
}

void ar::gui::settings_dialog::populate_paths()
{
    // ****************** Developer : Virtual Console & ROMs Paths ******************
    auto vc_path_res = _settings_manager->get_setting("vc_path");
    if (vc_path_res != std::nullopt)
    {
        ui->vc_path_line_edit->setText(QString::fromStdString(vc_path_res.value()));
    }

    auto roms_path_res = _settings_manager->get_setting("roms_path");
    if (roms_path_res != std::nullopt)
    {
        ui->rom_path_line_edit->setText(QString::fromStdString(roms_path_res.value()));
    }
}

void ar::gui::settings_dialog::populate_checkboxes()
{
    // ****************** General : Other ******************
    auto show_all_systems_res = _settings_manager->get_setting("show_all_systems");
    if (show_all_systems_res != std::nullopt)
    {
        std::string value = show_all_systems_res.value();

        ui->show_all_systems_checkbox->setCheckState(value == "true" ? Qt::CheckState::Checked :
                                                     Qt::CheckState::Unchecked);
    }
}
