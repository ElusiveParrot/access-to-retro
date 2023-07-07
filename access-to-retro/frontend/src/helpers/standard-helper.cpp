#include <unordered_map>
#include "standard-helper.hpp"
#include "qt-helper.hpp"

std::string ar::helpers::standard::get_console_icon_path_based_on_standard_name(const std::string& name)
{
    std::string expected_path = "consoles/" + QString::fromStdString(name).toLower().toStdString() + ".png";
    if (ar::helpers::qt::resources::check_if_resource_exists(expected_path.c_str()))
    {
        return ":/" + expected_path;
    }
    else
    {
        return ":/consoles/other.png";
    }
}

/// @brief Map for conversion between standard and common console name
static std::unordered_map<std::string, std::string> g_standard_common_name_map = {
        std::make_pair("n64", "Nintendo 64"),
        std::make_pair("game_boy", "GameBoy"),
        std::make_pair("game_boy_color", "GameBoy Color"),
        std::make_pair("game_gear", "Game Gear"),
        std::make_pair("genesis", "Sega Genesis"),
        std::make_pair("master_system", "Master System"),
        std::make_pair("mega_drive", "Mega Drive"),
        std::make_pair("nes", "NES"),
        std::make_pair("snes", "Super NES"),
        std::make_pair("atari_2600", "Atari 2600"),
        std::make_pair("pc_engine", "PC Engine"),
};

std::string ar::helpers::standard::get_console_label_from_standard_name(const std::string& standard_name)
{
    std::string standard_name_lower = QString::fromStdString(standard_name).toLower().toStdString();

    if (g_standard_common_name_map.find(standard_name_lower) != g_standard_common_name_map.end())
    {
        return g_standard_common_name_map[standard_name_lower];
    }
    else
    {
        // Not a standard name, return it back
        return standard_name;
    }
}

std::vector<std::string> ar::helpers::standard::get_list_of_standard_console_names()
{
    std::vector<std::string> names;
    names.reserve(g_standard_common_name_map.size());

    for (const auto& key_value : g_standard_common_name_map)
    {
        names.push_back(key_value.first);
    }

    return names;
}
