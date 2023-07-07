#include <QFileDialog>
#include <QList>
#include <QFile>
#include <utility>
#include "helpers/io-helper.hpp"
#include "helpers/qt-helper.hpp"
#include "util/logger.hpp"
#include "gui/widgets/virtual-console-selection-item-widget.hpp"
#include "gui/widgets/game-selection-item-widget.hpp"
#include "manager-dialog.hpp"
#include "ui_manager-dialog.h"

ar::gui::manager_dialog::manager_dialog(std::vector<std::unique_ptr<ar::core::virtual_console> >& virtual_consoles,
                                        std::vector<std::unique_ptr<ar::core::executable_binary> >& games,
                                        std::function<void()> on_change_fn, QWidget* parent) :
        QDialog(parent),
        _on_change_fn(std::move(on_change_fn)),
        _virtual_consoles_ref(virtual_consoles),
        _games_ref(games),
        ui(new Ui::manager_dialog)
{
    _settings_manager = ar::util::settings_manager::get_global_manager();

    ui->setupUi(this);

    populate_lists();
}

ar::gui::manager_dialog::~manager_dialog()
{
    delete ui;
}

void ar::gui::manager_dialog::on_delete_selected_game_button_clicked()
{
    QList<QModelIndex> selected_game_index = ui->games_list->selectionModel()->selectedIndexes().toVector();
    // If something is selected
    if (!selected_game_index.isEmpty())
    {
        auto selected_game_row = static_cast<uint64_t>(selected_game_index[0].row());
        std::unique_ptr<ar::core::executable_binary>& selected_game = _games_ref[selected_game_row];

        // Get path to delete file from selected game
        std::string path_to_delete = selected_game->get_path();

        // Delete file
        if (QFile::remove(QString::fromStdString(path_to_delete)))
        {
            // Callback due to change
            _on_change_fn();
        }
        else
        {
            // Delete failed
            ar::helpers::qt::show_error_message_box("Error!", "Unable to delete selected game!");
        }

        populate_lists();
    }
}

void ar::gui::manager_dialog::on_delete_selected_vc_button_clicked()
{
    QList<QModelIndex> selected_vc_index = ui->vcs_list->selectionModel()->selectedIndexes().toVector();
    // If something is selected
    if (!selected_vc_index.isEmpty())
    {
        auto selected_vc_row = static_cast<uint64_t>(selected_vc_index[0].row());
        std::unique_ptr<ar::core::virtual_console>& selected_vc = _virtual_consoles_ref[selected_vc_row];

        // Get path to delete file from selected vc
        std::string path_to_delete = selected_vc->get_path();

        // Delete file
        if (QFile::remove(QString::fromStdString(path_to_delete)))
        {
            // Callback due to change
            _on_change_fn();
        }
        else
        {
            // Delete failed
            ar::helpers::qt::show_error_message_box("Error!", "Unable to delete selected virtual console!");
        }

        populate_lists();
    }
}

void ar::gui::manager_dialog::on_install_new_game_button_clicked()
{
    QString selected_game_path = QFileDialog::getOpenFileName(this, ("Select Game File"), QDir::homePath());
    if (!selected_game_path.isEmpty())
    {
        // Get user's defined roms path
        auto games_path_res = _settings_manager->get_setting("roms_path");
        if (games_path_res == std::nullopt)
        {
            ar::helpers::qt::show_error_message_box("Error!", "Please set paths in the settings first!");
            return;
        }

        std::string games_path = games_path_res.value();

        try
        {
            ar::helpers::io::copy_file(selected_game_path.toStdString(), games_path);

            // Callback due to change
            _on_change_fn();
        }
        catch (const ar::error::io_error&)
        {
            // Copy failed
            ar::helpers::qt::show_error_message_box("Error!", "Unable to install selected game!");
        }

        populate_lists();
    }
}

void ar::gui::manager_dialog::on_install_new_vc_button_clicked()
{
    QString selected_vc_path = QFileDialog::getOpenFileName(this, ("Select Virtual Console"), QDir::homePath());
    if (!selected_vc_path.isEmpty())
    {
        // Get user's defined vcs path
        auto vc_path_res = _settings_manager->get_setting("vc_path");
        if (vc_path_res == std::nullopt)
        {
            ar::helpers::qt::show_error_message_box("Error!", "Please set paths in the settings first!");
            return;
        }

        std::string vc_path = vc_path_res.value();

        try
        {
            ar::helpers::io::copy_file(selected_vc_path.toStdString(), vc_path);

            // Callback due to change
            _on_change_fn();
        }
        catch (const ar::error::io_error& ex)
        {
            // Copy failed
            ar::helpers::qt::show_error_message_box("Error!", "Unable to install selected virtual console!",
                                                    QString::fromStdString(ex.get_error()));
        }

        populate_lists();
    }
}

void ar::gui::manager_dialog::populate_lists()
{
    // Clear old data first
    ui->vcs_list->clear();
    ui->games_list->clear();

    // ****************** Virtual Consoles ******************
    for (std::unique_ptr<ar::core::virtual_console>& virtual_console : _virtual_consoles_ref)
    {
        auto item = new QListWidgetItem(ui->vcs_list);
        item->setSizeHint(QSize(320, 80));

        ui->vcs_list->addItem(item);
        ui->vcs_list->setItemWidget(item, new ar::gui::virtual_console_selection_item_widget
                (virtual_console));
    }

    // ****************** Games ******************
    for (std::unique_ptr<ar::core::executable_binary>& game : _games_ref)
    {
        auto item = new QListWidgetItem(ui->games_list);
        item->setSizeHint(QSize(320, 80));

        ui->games_list->addItem(item);
        ui->games_list->setItemWidget(item, new ar::gui::game_selection_item_widget(game));
    }
}
