/**
 * @file graphics.h
 */

/** @defgroup group_game Game
 *  Helper functions dealing with game files, game loading/unloading
 *  @{
 */

#ifndef ACCESS_TO_RETRO_GAME_H
#define ACCESS_TO_RETRO_GAME_H

#include "data.h"

/**
 * @brief Represents an executable binary file
 * @remarks Used by the frontend application, can be used as a helper structure if you're a virtual console developer
 */
struct ar_executable
{
    /**
     * @brief Array of bytes of the binary file
     */
    const ar_byte* raw_bytes;

    /// @brief Size of the executable in bytes (size of raw_bytes array)
    size_t size;
};

/**
 * @brief Connect access to retro library with executable loaded in virtual console
 * @remarks Used by the frontend application, shouldn't be used by virtual console developers
 * @param bytes Pointer to raw bytes of the executable
 * @param size Size of the executable
 */
AR_API void ar_create_executable(const ar_byte* bytes, size_t size);

/**
 * @brief Getter for set executable, contains the binary of the game
 * @return Set executable object
 */
AR_API struct ar_executable* ar_get_executable(void);

/**
 * @brief Free the global library's executable object
 * @remarks Used by the frontend application, shouldn't be used by virtual console developers
 * @remark This gets called by 'ar_quit' so no need to call this directly in virtual console
 */
AR_API void ar_free_executable(void);

#endif //ACCESS_TO_RETRO_GAME_H

/** @} */ // end of group
