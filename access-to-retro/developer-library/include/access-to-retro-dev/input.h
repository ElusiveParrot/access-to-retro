/**
 * @file input.h
 */

/** @defgroup group_input Input
 *  Helper functions dealing with input handling
 *  @{
 */

#ifndef ACCESS_TO_RETRO_INPUT_H
#define ACCESS_TO_RETRO_INPUT_H

#include <stdbool.h>
#include "basics.h"

/// @brief Represents number of keys of Unified Access to Retro Controller
#define AR_UNIFIED_CONTROLLER_KEY_COUNT 24

/// @brief Enum defining every single key on Unified Access to Retro Controller
enum ar_unified_controller_key
{
    ar_unified_controller_left_analog_click,
    ar_unified_controller_left_analog_up,
    ar_unified_controller_left_analog_left,
    ar_unified_controller_left_analog_right,
    ar_unified_controller_left_analog_down,

    ar_unified_controller_right_analog_click,
    ar_unified_controller_right_analog_up,
    ar_unified_controller_right_analog_left,
    ar_unified_controller_right_analog_right,
    ar_unified_controller_right_analog_down,

    ar_unified_controller_dpad_up,
    ar_unified_controller_dpad_left,
    ar_unified_controller_dpad_right,
    ar_unified_controller_dpad_down,

    ar_unified_controller_x,
    ar_unified_controller_y,
    ar_unified_controller_b,
    ar_unified_controller_a,

    ar_unified_controller_start,
    ar_unified_controller_back,

    ar_unified_controller_left_bumper,
    ar_unified_controller_left_trigger,
    ar_unified_controller_right_bumper,
    ar_unified_controller_right_trigger
};

/// @brief Enum defining a state that a key on Unified Access to Retro Controller can have
enum ar_unified_controller_key_status
{
    /// @brief Key is NOT pressed
    ar_key_status_released,

    /// @brief Key is pressed
    ar_key_status_pressed
};

/**
 * @brief Initialises Access to Retro input API
 * @remarks Called in 'ar_init', no need for developers to call it themselves
 */
AR_API void ar_initialise_input_api(void);

/**
 * @brief Get status of a key on Unified Access to Retro Controller
 * @return Status of the provided key
 */
AR_API enum ar_unified_controller_key_status ar_get_unified_controller_key_status(enum ar_unified_controller_key key);

/**
 * @brief Set status of key on Unified Access to Retro Controller
 * @remarks Used by the frontend application, shouldn't be used by virtual console developers
 * @param key Key that will change its status
 * @param status Status to change to
 */
AR_API void ar_set_unified_controller_key_status(enum ar_unified_controller_key key,
                                                 enum ar_unified_controller_key_status status);

#endif //ACCESS_TO_RETRO_INPUT_H

/** @} */ // end of group
