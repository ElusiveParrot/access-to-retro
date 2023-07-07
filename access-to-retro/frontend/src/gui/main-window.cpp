#include <QToolButton>
#include <QMessageBox>
#include <QLabel>
#include <optional>
#include "gui/windows/virtual-console-and-game-picker-dialog.hpp"
#include "gui/widgets/game-selection-item-widget.hpp"
#include "gui/windows/first-time-setup-dialog.hpp"
#include "gui/windows/input-settings-dialog.hpp"
#include "gui/windows/settings-dialog.hpp"
#include "gui/windows/manager-dialog.hpp"
#include "gui/windows/about-dialog.hpp"
#include "helpers/cross-platform-helper.hpp"
#include "helpers/standard-helper.hpp"
#include "helpers/io-helper.hpp"
#include "helpers/qt-helper.hpp"
#include "util/logger.hpp"
#include "main-window.hpp"
#include "ui_main-window.h"

ar::gui::main_window::main_window(QWidget* parent) :
        QMainWindow(parent),
        ui(new Ui::main_window)
{
    _settings_manager = ar::util::settings_manager::get_global_manager();

    ui->setupUi(this);

    if (is_first_time_setup_needed())
    {
        auto first_time_setup_dialog = new ar::gui::first_time_setup_dialog(this);

        first_time_setup_dialog->exec();
    }

    std::optional<std::string> default_source_tab_setting_res = _settings_manager->get_setting("default_source_tab");
    if (default_source_tab_setting_res != std::nullopt)
    {
        int new_index = std::stoi(default_source_tab_setting_res.value());
        ui->launchable_tab_widget->setCurrentIndex(new_index);
    }

    // TODO: Add buttons to install and delete virtual consoles (move to path or remove)

    load_virtual_consoles();
    load_games();

    populate_systems_tab();
    populate_games_tab();
}

ar::gui::main_window::~main_window()
{
    delete ui;
}

void ar::gui::main_window::on_settings_button_clicked()
{
    // No need for RAII, Qt's will automatically deallocate it on exit
    auto* settings_window = new ar::gui::settings_dialog(this);

    settings_window->exec();

    on_refresh_button_clicked();
}

void ar::gui::main_window::on_manager_button_clicked()
{
    auto manager_window = new ar::gui::manager_dialog(_virtual_consoles, _games, [&]
    {
        // Reload games and virtual consoles whenever something gets installed/deleted
        load_virtual_consoles();
        load_games();
    }, this);

    manager_window->exec();

    // Simulate refresh button clicked
    on_refresh_button_clicked();
}

void ar::gui::main_window::on_input_button_clicked()
{
    auto manager_window = new ar::gui::input_settings_dialog(this);

    manager_window->exec();
}

void ar::gui::main_window::on_refresh_button_clicked()
{
    // Delete children of each tab's scroll area widget
    qDeleteAll(ui->systems_scroll_area_widget->findChildren<QWidget*>(Qt::FindDirectChildrenOnly));

    ui->games_list->clear();

    // Re-load virtual consoles and games
    load_virtual_consoles();
    load_games();

    // Repopulate tabs
    populate_systems_tab();
    populate_games_tab();

    // Tell Qt to repaint the Ui
    update();

    LOG_DEBUG("gui.main_window", "Ui refreshed");
}

std::vector<std::reference_wrapper<std::unique_ptr<ar::core::executable_binary> > >
ar::gui::main_window::collect_games_with_extensions(const std::vector<std::string>& extensions)
{
    bool skipped = false;

    std::vector<std::reference_wrapper<std::unique_ptr<ar::core::executable_binary> > > associated_games;
    for (std::unique_ptr<ar::core::executable_binary>& game : _games)
    {
        for (const std::string& extension : extensions)
        {
            if (ar::helpers::io::is_extension_part_of_extensions_string(game->get_file_extension(),
                                                                        extension))
            {
                associated_games.emplace_back(game);

                LOG_DEBUG("gui.main_window", "Adding game: '" + game->get_file_name() + "' to available game list");

                break;
            }
            else
            {
                if (!skipped)
                {
                    LOG_DEBUG("gui.main_window",
                              "Skipping game: '" + game->get_file_name() + "' due to wrong extension");
                    skipped = true;
                }
            }
        }
    }

    return associated_games;
}

void ar::gui::main_window::system_selected()
{
    auto clicked_button = qobject_cast<QToolButton*>(sender());

    std::string system_name = clicked_button->property("ar_system_name").value<QString>().toStdString();

    // Collect virtual consoles with that system name and collect extensions
    std::vector<std::reference_wrapper<std::unique_ptr<ar::core::virtual_console> > > associated_vcs;
    std::vector<std::string>                                                          associated_vcs_extensions;
    for (std::unique_ptr<ar::core::virtual_console>& vc : _virtual_consoles)
    {
        if (vc->get_system() == system_name)
        {
            associated_vcs.emplace_back(vc);
            associated_vcs_extensions.push_back(vc->get_rom_extension());

            LOG_DEBUG("gui.main_window", "Adding virtual console: '" + vc->get_name() + "' to available virtual"
                                                                                        "console list");
        }
    }

    // Collect games
    std::vector<std::reference_wrapper<std::unique_ptr<ar::core::executable_binary> > >
            associated_games = collect_games_with_extensions(associated_vcs_extensions);
    if (associated_vcs.empty())
    {
        ar::helpers::qt::show_error_message_box("Error!", "No virtual cores found for this system!",
                                                "Please see the guide on how to install them.",
                                                QSize(500, 200), this);
        return;
    }

    // No need for RAII, Qt will automatically deallocate it on exit
    auto vc_and_game_picker = new ar::gui::virtual_console_and_game_picker_dialog(
            associated_vcs, associated_games, associated_games.empty(), this);

    vc_and_game_picker->exec();
}

void ar::gui::main_window::on_about_button_clicked()
{
    auto about_window = new ar::gui::about_dialog(this);

    about_window->exec();
}

void ar::gui::main_window::on_games_search_button_clicked()
{
    // Clear old list first
    ui->games_list->clear();

    std::string query = ui->games_search_textbox->text().toStdString();
    populate_games_tab(query);
}

void ar::gui::main_window::on_games_list_currentItemChanged(QListWidgetItem* item)
{
    // virtual console and game picker expects a vector so create this wrapper
    std::vector<std::reference_wrapper<std::unique_ptr<ar::core::executable_binary> > > game_as_vector;

    // Cast selected widget to its actual class
    auto selected_widget = (ar::gui::game_selection_item_widget*) (ui->games_list->itemWidget(item));

    // Add it to vector
    std::unique_ptr<ar::core::executable_binary>& selected_game = selected_widget->get_game();
    game_as_vector.emplace_back(selected_game);

    // Collect associated virtual consoles
    std::vector<std::reference_wrapper<std::unique_ptr<ar::core::virtual_console> > > associated_vcs;
    for (std::unique_ptr<ar::core::virtual_console>& vc : _virtual_consoles)
    {
        if (ar::helpers::io::is_extension_part_of_extensions_string(selected_game->get_file_extension(),
                                                                    vc->get_rom_extension()))
        {
            associated_vcs.emplace_back(vc);
        }
    }

    if (associated_vcs.empty())
    {
        ar::helpers::qt::show_error_message_box("Error!", "No virtual cores found for this game!",
                                                "Please see the guide on how to install them.",
                                                QSize(500, 200), this);
        return;
    }

    // No need for RAII, Qt will automatically deallocate it on exit
    auto vc_and_game_picker = new ar::gui::virtual_console_and_game_picker_dialog(
            associated_vcs, game_as_vector, true, this);

    vc_and_game_picker->exec();

    // TODO: Clear selection
}

void ar::gui::main_window::on_games_search_clear_filter_button_clicked()
{
    // Clear text from search box
    ui->games_search_textbox->setText("");

    // Simulate search button click
    on_games_search_button_clicked();
}

void ar::gui::main_window::load_virtual_consoles()
{
    // Reset vector
    _virtual_consoles.clear();

    // If error then log as critical or error, don't fail
    std::optional<std::string> path_res = _settings_manager->get_setting("vc_path");
    if (path_res == std::nullopt)
    {
        LOG_CRITICAL("Setting with key 'vc_path' not found while it is expected. Possibly hand-modified config file");
        return;
    }

    QDir        directory(QString::fromStdString(path_res.value()));
    QStringList vc_paths = directory.entryList(QStringList() << "*.vc" << "*.VC", QDir::Files);

    for (const QString& vc_file_name : vc_paths)
    {
        // Validate that this vc is compiled for the platform and skip if it's not
        if (!ar::helpers::cross_platform::check_vc_os_compatibility(vc_file_name.toStdString()))
        {
            LOG_INFO("Virtual console file: '" + vc_file_name.toStdString() + "' rejected in compatibility check.");
            continue;
        }

        QString vc_path = directory.absoluteFilePath(vc_file_name);
        try
        {
            _virtual_consoles.push_back(std::make_unique<ar::core::virtual_console>(vc_path.toStdString()));
        }
        catch (const ar::core::virtual_console_error& ex)
        {
            LOG_CRITICAL("Unable to create virtual console from path: '" + vc_path.toStdString() + "' due to error:" +
                         "\n" + ex.get_logger_formatted_error());

            // TODO: Show modal instead?

            continue;
        }
        catch (const ar::error::io_error& ex)
        {
            LOG_CRITICAL("Unable to open '.vc' file from path: '" + vc_path.toStdString() + "' due to error:" +
                         "\n" + ex.get_logger_formatted_error());
            continue;
        }
    }
}

void ar::gui::main_window::load_games()
{
    // Reset vector
    _games.clear();

    // If error then log as critical or error, don't fail
    std::optional<std::string> path_res = _settings_manager->get_setting("roms_path");
    if (path_res == std::nullopt)
    {
        LOG_CRITICAL("Setting with key 'roms_path' not found while it is expected. Possibly hand-modified config file");
        return;
    }

    QDir        directory(QString::fromStdString(path_res.value()));
    QStringList game_list = directory.entryList(QDir::Files);

    for (const QString& game_name : game_list)
    {
        QString game_path = directory.absoluteFilePath(game_name);

        try
        {
            // Emplace back creates ar::core::executable_binary object automatically based on arguments
            _games.push_back(std::make_unique<ar::core::executable_binary>(game_path.toStdString()));
        }
        catch (const ar::error::io_error& ex)
        {
            LOG_CRITICAL("Unable to create game object from path: '" + game_path.toStdString() + "' due to error:" +
                         "\n" + ex.get_logger_formatted_error());
            continue;
        }
    }
}

static QToolButton* generate_button_for_system(const std::string& standard_name)
{
    auto button = new QToolButton(); // No need for RAII, will be deallocated automatically by Qt
    button->setToolButtonStyle(Qt::ToolButtonStyle::ToolButtonTextUnderIcon);
    button->setStyleSheet("background-color: rgba(0,0,0,0);");

    button->setText(QString::fromStdString(
            ar::helpers::standard::get_console_label_from_standard_name(standard_name)));

    QFont font = button->font();
    font.setPointSize(14);
    button->setFont(font);

    button->setIcon(QIcon(QString::fromStdString(ar::helpers::standard::get_console_icon_path_based_on_standard_name
                                                         (standard_name))));
    button->setIconSize(QSize(100, 100));

    return button;
}

bool ar::gui::main_window::is_first_time_setup_needed()
{
    std::optional<std::string> setting = _settings_manager->get_setting("first_time_setup_done");
    return !(setting != std::nullopt && setting.value() == "true");
}

void ar::gui::main_window::populate_systems_tab()
{
    QLayout* systems_scroll_area_layout = ui->systems_scroll_area_widget->layout();

    std::vector<std::string> installed_vc_system_names;

    // If "show_all_systems" enabled in settings then add all standard consoles even if no virtual consoles found
    auto show_all_systems_res = _settings_manager->get_setting("show_all_systems");
    if (show_all_systems_res != std::nullopt && show_all_systems_res.value() == "true")
    {
        std::vector<std::string> standard_names = ar::helpers::standard::get_list_of_standard_console_names();
        for (const std::string& standard_name : standard_names)
        {
            installed_vc_system_names.push_back(standard_name);
        }
    }

    for (const std::unique_ptr<ar::core::virtual_console>& vc : _virtual_consoles)
    {
        // Only add if not exists
        std::string system_name = vc->get_system();
        if (std::find(installed_vc_system_names.begin(), installed_vc_system_names.end(), system_name) ==
            installed_vc_system_names.end())
        {
            installed_vc_system_names.push_back(system_name);
        }
    }

    // No system installed, show message
    if (installed_vc_system_names.empty())
    {
        auto label = new QLabel("No Virtual Consoles installed, please check your path in settings!");
        label->setAlignment(Qt::AlignmentFlag::AlignCenter);
        systems_scroll_area_layout->addWidget(label);
    }
    else
    {
        int row    = 0;
        int column = 0;

        // Add button for each system
        for (const std::string& system_name : installed_vc_system_names)
        {
            QToolButton* button = generate_button_for_system(system_name);
            // Set custom property to know what system name is clicked
            button->setProperty("ar_system_name", QString::fromStdString(system_name));

            // Connect it with an action
            QObject::connect(button, &QToolButton::clicked, this,
                             &ar::gui::main_window::system_selected);

            // Add to grid layout, cast to more specific inherited class QGridLayout so that row and column can be added
            dynamic_cast<QGridLayout*>(systems_scroll_area_layout)->addWidget(button, row, column);

            // Update column, max columns = 6
            column++;
            if (column == 6)
            {
                row++;
                column = 0;
            }
        }
    }
}

void ar::gui::main_window::populate_games_tab(const std::string& filter)
{
    // Create widget in the list for each game
    for (std::unique_ptr<ar::core::executable_binary>& game : _games)
    {
        // If filter is provided BUT game name does not contain it
        if (!filter.empty() && game->get_file_name().find(filter) == std::string::npos)
        {
            continue;
        }

        auto item = new QListWidgetItem(ui->games_list);
        item->setSizeHint(QSize(320, 80));

        ui->games_list->addItem(item);
        ui->games_list->setItemWidget(item, new ar::gui::game_selection_item_widget(game));
    }
}
