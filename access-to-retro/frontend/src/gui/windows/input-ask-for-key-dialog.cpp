#include <QKeyEvent>
#include <utility>
#include "ui_input-ask-for-key-dialog.h"
#include "input-ask-for-key-dialog.hpp"

ar::gui::input_ask_for_key_dialog::input_ask_for_key_dialog(QWidget* parent, std::string key_setting_name) :
        QDialog(parent),
        _settings_manager(ar::util::settings_manager::get_global_manager()),
        _key_setting_name(std::move(key_setting_name)),
        ui(new Ui::input_ask_for_key_dialog)
{
    setFocusPolicy(Qt::StrongFocus);

    ui->setupUi(this);
}

ar::gui::input_ask_for_key_dialog::~input_ask_for_key_dialog()
{
    delete ui;
}

void ar::gui::input_ask_for_key_dialog::keyPressEvent(QKeyEvent* event)
{
    std::string key_name = QKeySequence(event->key()).toString().toStdString();

    _settings_manager->set_setting(_key_setting_name, key_name);

    QDialog::accept();
}
