#include "about-dialog.hpp"
#include "ui_about-dialog.h"

ar::gui::about_dialog::about_dialog(QWidget* parent) :
        QDialog(parent),
        ui(new Ui::about_dialog)
{
    ui->setupUi(this);
}

ar::gui::about_dialog::~about_dialog()
{
    delete ui;
}
