#include "helpers/standard-helper.hpp"
#include "virtual-console-selection-item-widget.hpp"
#include "ui_virtual-console-selection-item-widget.h"

ar::gui::virtual_console_selection_item_widget::virtual_console_selection_item_widget(
        std::unique_ptr<ar::core::virtual_console>& virtual_console, QWidget* parent) :
        QFrame(parent),
        _virtual_console(virtual_console),
        ui(new Ui::virtual_console_selection_item_widget)
{
    ui->setupUi(this);

    ui->name_label->setText(QString::fromStdString(_virtual_console->get_name()));
    ui->author_label->setText(QString::fromStdString(_virtual_console->get_author()));
    ui->rom_extensions_label->setText(QString::fromStdString(_virtual_console->get_rom_extension()));

    ui->icon_tool_button->setIcon(QIcon(QString::fromStdString
                                                (ar::helpers::standard::get_console_icon_path_based_on_standard_name(
                                                        _virtual_console->get_name()))));

    ui->icon_tool_button->setIconSize(QSize(60, 60));

    setFrameShape(QFrame::Shape::StyledPanel);
}

ar::gui::virtual_console_selection_item_widget::~virtual_console_selection_item_widget()
{
    delete ui;
}
