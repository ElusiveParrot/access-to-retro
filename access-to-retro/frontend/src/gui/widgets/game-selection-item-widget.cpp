#include "helpers/qt-helper.hpp"
#include "game-selection-item-widget.hpp"
#include "ui_game-selection-item-widget.h"

ar::gui::game_selection_item_widget::game_selection_item_widget(std::unique_ptr<ar::core::executable_binary>& game,
                                                                QWidget* parent) :
        QFrame(parent),
        _game(game),
        ui(new Ui::game_selection_item_widget)
{
    ui->setupUi(this);

    ui->name_label->setText(QString::fromStdString(_game->get_file_name()));
    ui->extension_label->setText(QString::fromStdString(_game->get_file_extension()));

    ui->icon_tool_button->setIcon(QIcon(QString::fromStdString(ar::helpers::qt::resources::GAME_ICON)));

    ui->icon_tool_button->setIconSize(QSize(60, 60));

    setFrameShape(QFrame::Shape::StyledPanel);
}

ar::gui::game_selection_item_widget::~game_selection_item_widget()
{
    delete ui;
}

std::unique_ptr<ar::core::executable_binary>& ar::gui::game_selection_item_widget::get_game()
{
    return _game;
}
