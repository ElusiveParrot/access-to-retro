#include "gameplay-window.hpp"
#include "gui/widgets/sdl-graphics-widget.hpp"
#include "ui_gameplay-window.h"


ar::gui::gameplay_window::gameplay_window(std::unique_ptr<ar::core::virtual_console>& virtual_console,
                                          std::unique_ptr<ar::core::executable_binary>& game,
                                          QWidget* parent) :
        QDialog(parent),
        ui(new Ui::gameplay_window)
{
    ui->setupUi(this);

    setWindowFlags(Qt::Window);

    setAttribute(Qt::WA_DeleteOnClose);

    setWindowTitle(QString::fromStdString(virtual_console->get_name()) + " : " +
                   QString::fromStdString(game->get_file_name()));

    // Resize window to match recommended by virtual console
    // TODO: In settings menu resolution and checkbox to allow recommended by emulator
    resize(static_cast<int>(virtual_console->get_default_window_width()),
           static_cast<int>(virtual_console->get_default_window_height()));

    ui->horizontalLayout->addWidget(new ar::gui::sdl_graphics_widget(virtual_console, game));
}

ar::gui::gameplay_window::~gameplay_window()
{
    delete ui;
}
