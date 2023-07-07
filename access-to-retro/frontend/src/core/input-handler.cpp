#include <QKeyEvent>
#include "util/logger.hpp"
#include "input-handler.hpp"

ar::core::input_handler::input_handler(std::unique_ptr<core::virtual_console>& virtual_console) :
        _settings_manager(ar::util::settings_manager::get_global_manager()),
        _virtual_console(virtual_console)
{
    fill_input_translation_map_from_settings();

    // Get key setting function from the library
    _set_key_status_fn = _virtual_console->get_symbol_from_library
            <void(*)(ar_unified_controller_key, ar_unified_controller_key_status)>("ar_set_unified_controller_key_status");
}

void ar::core::input_handler::key_press(const std::string& key)
{
    // This key is found inside settings - let virtual console know that Unified Controller button was pressed
    if (_input_translation_map.contains(key))
    {
        ar_unified_controller_key unified_key = _input_translation_map[key];

        _set_key_status_fn(unified_key, ar_key_status_pressed);
    }
}

void ar::core::input_handler::key_release(const std::string& key)
{
    // This key is found inside settings - let virtual console know that Unified Controller button was released
    if (_input_translation_map.contains(key))
    {
        ar_unified_controller_key unified_key = _input_translation_map[key];

        _set_key_status_fn(unified_key, ar_key_status_released);
    }
}

/**
 * @brief List of settings and their corresponding keys
 * @details This list contains a list of settings inside the settings file and the Unified Controller key that
 *          corresponds to it
 */
static std::pair<std::string, ar_unified_controller_key>
        key_setting_and_key_enum_list[AR_UNIFIED_CONTROLLER_KEY_COUNT] =
        {
                // Left analog stick
                { "input_left_analog_click",  ar_unified_controller_left_analog_click },
                { "input_left_analog_up",     ar_unified_controller_left_analog_up },
                { "input_left_analog_left",   ar_unified_controller_left_analog_left },
                { "input_left_analog_right",  ar_unified_controller_left_analog_right },
                { "input_left_analog_down",   ar_unified_controller_left_analog_down },
                // Right analog stick
                { "input_right_analog_click", ar_unified_controller_right_analog_click },
                { "input_right_analog_up",    ar_unified_controller_right_analog_up },
                { "input_right_analog_left",  ar_unified_controller_right_analog_left },
                { "input_right_analog_right", ar_unified_controller_right_analog_right },
                { "input_right_analog_down",  ar_unified_controller_right_analog_down },
                // Directional pad
                { "input_dpad_up",            ar_unified_controller_dpad_up },
                { "input_dpad_left",          ar_unified_controller_dpad_left },
                { "input_dpad_right",         ar_unified_controller_dpad_right },
                { "input_dpad_down",          ar_unified_controller_dpad_down },
                // Face buttons
                { "input_start",              ar_unified_controller_start },
                { "input_back",               ar_unified_controller_back },
                { "input_y",                  ar_unified_controller_y },
                { "input_x",                  ar_unified_controller_x },
                { "input_b",                  ar_unified_controller_b },
                { "input_a",                  ar_unified_controller_a },
                // Top buttons
                { "input_left_bumper",        ar_unified_controller_left_bumper },
                { "input_left_trigger",       ar_unified_controller_left_trigger },
                { "input_right_bumper",       ar_unified_controller_right_bumper },
                { "input_right_trigger",      ar_unified_controller_right_trigger },
        };

void ar::core::input_handler::fill_input_translation_map_from_settings()
{
    for (const auto& setting_key_pair : key_setting_and_key_enum_list)
    {
        auto setting = _settings_manager->get_setting(setting_key_pair.first);
        if (setting != std::nullopt)
        {
            _input_translation_map.insert({ setting.value(), setting_key_pair.second });
        }
    }
}
