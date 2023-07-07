#include <iostream>
#include "input-ask-for-key-dialog.hpp"
#include "input-settings-dialog.hpp"
#include "ui_input-settings-dialog.h"

ar::gui::input_settings_dialog::input_settings_dialog(QWidget* parent) :
        QDialog(parent),
        _settings_manager(ar::util::settings_manager::get_global_manager()),
        ui(new Ui::input_settings_dialog)
{
    ui->setupUi(this);

    connect_buttons();

    refresh_button_names();
}

ar::gui::input_settings_dialog::~input_settings_dialog()
{
    delete ui;
}

void ar::gui::input_settings_dialog::connect_buttons()
{
    for (QPushButton* button : ui->settings_group_box->findChildren<QPushButton*>())
    {
        // button's accessible name is used as setting name
        std::string setting_name = "input_" + button->accessibleName().toStdString();

        QObject::connect(button, &QPushButton::clicked, this, [&, setting_name]
        {
            // On click open window for setting new key
            // No need for RAII, Qt will auto delete input_ask_for_key_dialogs
            auto key_ask_window = new ar::gui::input_ask_for_key_dialog(this, setting_name);

            key_ask_window->exec();

            refresh_button_names();
        });
    }
}

void ar::gui::input_settings_dialog::refresh_button_names()
{
    for (QPushButton* button : ui->settings_group_box->findChildren<QPushButton*>())
    {
        // Set button key name to currently set key
        std::string                setting_name    = "input_" + button->accessibleName().toStdString();
        std::optional<std::string> current_setting = _settings_manager->get_setting(setting_name);

        button->setText(QString::fromStdString(current_setting == std::nullopt ? "Click to Set Button" :
                                               current_setting.value()));
    }
}
