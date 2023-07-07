#include <access-to-retro-dev/access-to-retro-dev.h>

/****************************************************************************************************
 API global objects
****************************************************************************************************/

/**
 * @brief Array containing status of each Unified Access to Retro Controller key status
 * @details Each key in 'ar_unified_controller_key' enum when casted to integer represents index of that key
 *          in this array.
 */
static volatile enum ar_unified_controller_key_status g_unified_controller_key_status[AR_UNIFIED_CONTROLLER_KEY_COUNT];


/****************************************************************************************************
 API Implementation
****************************************************************************************************/

AR_API void ar_initialise_input_api(void)
{
    // Set each key to NOT pressed
    for (int i = 0; i < AR_UNIFIED_CONTROLLER_KEY_COUNT; i++)
    {
        g_unified_controller_key_status[i] = ar_key_status_released;
    }
}

AR_API enum ar_unified_controller_key_status ar_get_unified_controller_key_status(enum ar_unified_controller_key key)
{
    // See 'g_unified_controller_key_status' doc comment for explanation of casting to int
    return g_unified_controller_key_status[(int) key];
}

AR_API void ar_set_unified_controller_key_status(enum ar_unified_controller_key key,
                                                 enum ar_unified_controller_key_status status)
{
    // See 'g_unified_controller_key_status' doc comment for explanation of casting to int
    g_unified_controller_key_status[(int) key] = status;
}
