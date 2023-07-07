/**
 * @file graphics.h
 */

/** @defgroup group_graphics Graphics
 *  Deals with graphical output/input
 *  @{
 */

#ifndef ACCESS_TO_RETRO_GRAPHICS_H
#define ACCESS_TO_RETRO_GRAPHICS_H

#include <SDL.h>
#include "basics.h"
#include "data.h"

/// @brief Represents OpenGL context object
typedef ar_generic_memory_ptr open_gl_context;

/// @brief Contains all methods of supported graphical output, should be set using 'ar_graphics_set_method'
enum ar_graphics_method
{
    /// @brief No graphical output
    ar_graphics_method_none,

    /// @brief Output graphics using a raw frame buffer
    ar_graphics_method_frame_buffer,

    /// @brief Output graphics using SDL2 renderer
    ar_graphics_method_sdl,

    /// @brief Output graphics using OpenGL directly
    ar_graphics_method_open_gl_context,
};

/**
 * @brief Set graphics method to be used by this virtual console
 * @remark It is recommended to call this function in AR_STARTUP_FN
 * @warning If this is not set then 'ar_graphics_method_none' is assumed
 */
AR_API void ar_graphics_set_method(enum ar_graphics_method method);

/**
 * @brief Get currently set graphics method
 * @return Currently set graphics method
 */
AR_API enum ar_graphics_method ar_graphics_get_method(void);

/**
 * @brief Free the graphical object based on current graphical method
 * @remarks Called automatically in 'ar_quit', no need for developer to call it himself
 * @remark This gets called by 'ar_quit' so no need to call this directly in virtual console
 */
AR_API void ar_graphics_free_object(void);

/************************************* Frame Buffer *************************************/

/// @brief Represents a single pixel inside 'ar_frame_buffer', can be used for other purposes by developers
struct ar_pixel
{
    /// @brief Value of red colour
    uint8_t r;

    /// @brief Value of green colour
    uint8_t g;

    /// @brief Value of blue colour
    uint8_t b;

    /// @brief Value of opacity
    uint8_t a;
};

/// @brief Represents a frame buffer (array of pixels) that is used for 'ar_graphics_method_framebuffer' graphics method
struct ar_frame_buffer
{
    /// @brief Array of individual pixels (size = .width x .height)
    struct ar_pixel* pixels;

    /// @brief Width of the frame
    uint32_t width;

    /// @brief Height of the frame
    uint32_t height;
};

/**
 * @brief Create a frame buffer graphical object
 * @details This NEEDS to be called if graphics method is set to 'ar_graphics_method_frame_buffer' otherwise getter will
 *          return nullptr.
 * @param width Width of the frame
 * @param height Height of the frame
 */
AR_API void ar_graphics_create_frame_buffer(uint32_t width, uint32_t height);

/**
 * @brief Getter for frame buffer graphical object
 * @warning If 'ar_graphics_create_frame_buffer' was not called before then this will return nullptr.
 * @return Pointer to the previously created frame buffer graphical object
 */
AR_API struct ar_frame_buffer* ar_graphics_get_frame_buffer(void);

/************************************* SDL2 *************************************/

/**
 * @brief Connect access to retro library with access to retro frontend SDL objects
 * @remarks Used by the frontend application, shouldn't be used by virtual console developers
 * @param window Frontend's SDL Window object
 * @param renderer Frontend's SDL Renderer object
 */
AR_API void ar_graphics_set_sdl_objects(SDL_Window* window, SDL_Renderer* renderer);

/**
 * @brief Getter for frontend's SDL window graphical object
 * @warning If 'ar_graphics_set_sdl_objects' was not called before then this will return nullptr.
 * @return Pointer to frontend's SDL window object
 */
AR_API SDL_Window* ar_graphics_get_sdl_window(void);

/**
 * @brief Getter for frontend's SDL window renderer object
 * @warning If 'ar_graphics_set_sdl_objects' was not called before then this will return nullptr.
 * @return Pointer to frontend's SDL renderer object
 */
AR_API SDL_Renderer* ar_graphics_get_sdl_renderer(void);

/************************************* OpenGL *************************************/

/**
 * @brief Connect access to retro library with frontend's OpenGL context
 * @remarks Used by the frontend application, shouldn't be used by virtual console developers
 * @param context Frontend's OpenGL context
 */
AR_API void ar_graphics_set_gl_context(open_gl_context context);

/**
 * @brief Getter for frontend's OpenGL context
 * @warning If 'ar_graphics_set_sdl_objects' was not called before then this will return nullptr.
 * @return Pointer to frontend's SDL window object
 */
AR_API open_gl_context ar_graphics_get_gl_context(void);

#endif //ACCESS_TO_RETRO_GRAPHICS_H

/** @} */ // end of group
