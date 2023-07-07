#include <QSpacerItem>
#include <QMainWindow>
#include <utility>
#include <string>
#include <QMessageBox>
#include <SDL.h>
#include "gui/windows/gameplay-window.hpp"
#include "gui/widgets/virtual-console-selection-item-widget.hpp"
#include "gui/widgets/game-selection-item-widget.hpp"
#include "helpers/io-helper.hpp"
#include "helpers/qt-helper.hpp"
#include "virtual-console-and-game-picker-dialog.hpp"
#include "ui_virtual-console-and-game-picker-dialog.h"

ar::gui::virtual_console_and_game_picker_dialog::virtual_console_and_game_picker_dialog(
        std::vector<std::reference_wrapper<std::unique_ptr<ar::core::virtual_console> > > associated_virtual_consoles,
        std::vector<std::reference_wrapper<std::unique_ptr<ar::core::executable_binary> > > associated_games,
        bool hide_games, QWidget* parent) :
        QDialog(parent),
        _associated_virtual_consoles(std::move(associated_virtual_consoles)),
        _associated_games(std::move(associated_games)),
        ui(new Ui::virtual_console_and_game_picker_dialog)
{
    ui->setupUi(this);

    // Populate virtual consoles list even if only one (or none) present as the one on the top will be chosen as
    // default if none selected
    populate_virtual_consoles_selection();

    if (hide_games)
    {
        ui->game_list->hide();
    }

    // Populate games list even if only one (or none) present as the one on the top will be chosen as default if none
    // selected
    populate_games_selection();

    // TODO: If one of vectors is empty then dont start virtual console window, instead show a modal
}

ar::gui::virtual_console_and_game_picker_dialog::~virtual_console_and_game_picker_dialog()
{
    delete ui;
}

void ar::gui::virtual_console_and_game_picker_dialog::accept()
{
    // TODO: Note if selected returns -1 then probably not selected - use top index 0

    if (_associated_virtual_consoles.empty())
    {
        ar::helpers::qt::show_error_message_box("Error!", "No virtual cores found for this system!",
                                                "Please see the guide on how to install them",
                                                QSize(500, 200), this);
        return;
    }

    if (_associated_games.empty())
    {
        ar::helpers::qt::show_error_message_box("Error!", "No games found for this system!",
                                                "Please see the guide on how to install them",
                                                QSize(500, 200), this);
        return;
    }

    QList<QModelIndex> selected_vc_index   = ui->virtual_console_list->selectionModel()->selectedIndexes().toVector();
    QList<QModelIndex> selected_game_index = ui->game_list->selectionModel()->selectedIndexes().toVector();


    // If select_vc/game_index (QList) is empty then nothing was chosen = use top one
    // There is no multi-select so get [0] index.
    std::unique_ptr<ar::core::virtual_console>& selected_vc = _associated_virtual_consoles[
            selected_vc_index.isEmpty() ? 0 : static_cast<uint64_t>(selected_vc_index[0].row())];

    std::unique_ptr<ar::core::executable_binary>& selected_game = _associated_games[
            selected_game_index.isEmpty() ? 0 :
            static_cast<uint64_t>(selected_game_index[0].row())];

    launch_virtual_console_with_game(selected_vc, selected_game);

    QDialog::accept();
}

void ar::gui::virtual_console_and_game_picker_dialog::populate_games_selection()
{
    for (std::unique_ptr<ar::core::executable_binary>& game : _associated_games)
    {
        auto item = new QListWidgetItem(ui->game_list);
        item->setSizeHint(QSize(320, 80));

        ui->game_list->addItem(item);
        ui->game_list->setItemWidget(item, new ar::gui::game_selection_item_widget(game));
    }
}

void ar::gui::virtual_console_and_game_picker_dialog::populate_virtual_consoles_selection()
{
    for (std::unique_ptr<ar::core::virtual_console>& virtual_console : _associated_virtual_consoles)
    {
        auto item = new QListWidgetItem(ui->virtual_console_list);
        item->setSizeHint(QSize(320, 80));

        ui->virtual_console_list->addItem(item);
        ui->virtual_console_list->setItemWidget(item, new ar::gui::virtual_console_selection_item_widget
                (virtual_console));
    }
}

void ar::gui::virtual_console_and_game_picker_dialog::launch_virtual_console_with_game(
        std::unique_ptr<ar::core::virtual_console>& virtual_console,
        std::unique_ptr<ar::core::executable_binary>& game)
{
    // Game/virtual console picker is a child of main window so cast parent
    auto main_window = (QMainWindow*) parent();

    // Validate that extension matches
    if (!ar::helpers::io::is_extension_part_of_extensions_string(game->get_file_extension(),
                                                                 virtual_console->get_rom_extension()))
    {
        ar::helpers::qt::show_error_message_box("Error!", "Compatibility Check Failed",
                                                "Please check the file extensions that this virtual console supports or try a different one");

        return;
    }

    try
    {
        auto gameplay_window = new ar::gui::gameplay_window(virtual_console, game, main_window);

        gameplay_window->show();
    }
    catch (const ar::core::virtual_console_error& ex)
    {
        // Virtual Console threw an error
        ar::helpers::qt::show_error_message_box("Error!", "Virtual Console threw an exception!",
                                                QString::fromStdString(ex.get_error()));
    }
    catch (const std::runtime_error&)
    {
        ar::helpers::qt::show_error_message_box("Error!", "Third party library initialization failed!",
                                                QString::fromStdString(SDL_GetError()));
    }
}
