/**
 * @file core/input-handler.hpp
 */

#ifndef ACCESS_TO_RETRO_INPUT_HANDLER_HPP
#define ACCESS_TO_RETRO_INPUT_HANDLER_HPP

#include <access-to-retro-dev/access-to-retro-dev.h>
#include <unordered_map>
#include <memory>
#include "core/virtual-console.hpp"
#include "util/settings-manager.hpp"

namespace ar::core
{
    /// @brief Type representing ID of a Qt key (keyboard/gamepad)
    typedef int key_id;

    /// @brief Handles gamepad/keyboard inputs for the virtual console
    class input_handler
    {
    public:
        /**
         * @brief Default constructor
         * @param virtual_console Virtual console that the inputs will be directed to
         */
        explicit input_handler(std::unique_ptr<core::virtual_console>& virtual_console);

        /**
         * @brief Handles key press event
         * @param key Pressed key name
         */
        void key_press(const std::string& key);

        /**
         * @brief Handles key release event
         * @param key Released key name
         */
        void key_release(const std::string& key);

    private:
        /// @brief Global settings manager used to get user's input settings
        std::shared_ptr<ar::util::settings_manager> _settings_manager;

        /// @brief Virtual console that the inputs will be directed to
        std::unique_ptr<ar::core::virtual_console>                                       & _virtual_console;

        /// @brief Access to Retro library function to set key status, fetches from '_virtual_console' in constructor
        std::function<void(ar_unified_controller_key, ar_unified_controller_key_status)> _set_key_status_fn;

        /**
         * @brief Map used for input translation Qt -> Unified Controller
         * @details Whenever a button is pressed it will be translated from its string value into the enum
         *          of unified controller:
         *
         *          KEY_NAME (is =) CURRENTLY_PRESSED_KEY
         *          MAP[KEY_NAME] (gives ->) CONTROLLER_KEY
         *          CONTROLLER_KEY (is) MARKED AS PRESSED
         */
        std::unordered_map<std::string, ar_unified_controller_key> _input_translation_map;

        /// @brief Fill input translation map from settings
        void fill_input_translation_map_from_settings();
    };
}

#endif //ACCESS_TO_RETRO_INPUT_HANDLER_HPP
