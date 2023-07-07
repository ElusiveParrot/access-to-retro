#include <access-to-retro-dev/access-to-retro-dev.h>
#include <stdlib.h>

/****************************************************************************************************
 API global objects
****************************************************************************************************/

/// @brief Currently selected graphics method
static enum ar_graphics_method g_graphics_method = ar_graphics_method_none;

/// @brief Graphical object for 'ar_graphics_method_frame_buffer', nullptr if different mode is selected
static struct ar_frame_buffer* g_graphics_object_frame_buffer = NULL;

/// @brief Graphical object for 'ar_graphics_method_sdl', nullptr if different mode is selected
static SDL_Window* g_graphics_object_window = NULL;

/// @brief Graphical object for 'ar_graphics_method_sdl', nullptr if different mode is selected
static SDL_Renderer* g_graphics_object_renderer = NULL;

/// @brief Graphical object for 'ar_graphics_method_open_gl_context', nullptr if different mode is selected
static open_gl_context g_graphics_object_gl_context = NULL;

/****************************************************************************************************
 API Implementation
****************************************************************************************************/

AR_API void ar_graphics_set_method(enum ar_graphics_method method)
{
    g_graphics_method = method;
}

AR_API enum ar_graphics_method ar_graphics_get_method(void)
{
    return g_graphics_method;
}

AR_API void ar_graphics_free_object(void)
{
    switch (g_graphics_method)
    {
        case ar_graphics_method_frame_buffer:
            // Delete pixels first
            free(g_graphics_object_frame_buffer->pixels);
            free(g_graphics_object_frame_buffer);

            // TODO: Other (SDL not needed, does opengl need it?)

        default:; // Do nothing
    }
}

/************************************* Frame Buffer *************************************/

AR_API void ar_graphics_create_frame_buffer(uint32_t width, uint32_t height)
{
    //g_graphics_object_frame_buffer = new ar_frame_buffer;
    g_graphics_object_frame_buffer = malloc(sizeof(struct ar_frame_buffer));

    g_graphics_object_frame_buffer->width  = width;
    g_graphics_object_frame_buffer->height = height;

    g_graphics_object_frame_buffer->pixels = malloc(sizeof(struct ar_pixel) * width * height);

    // Fill the pixels with default value (black pixel with full opacity)
    for (uint32_t i = 0; i < width * height; i++)
    {
        g_graphics_object_frame_buffer->pixels[i].r = 0;
        g_graphics_object_frame_buffer->pixels[i].g = 0;
        g_graphics_object_frame_buffer->pixels[i].b = 0;
        g_graphics_object_frame_buffer->pixels[i].a = 255;
    }
}

AR_API struct ar_frame_buffer* ar_graphics_get_frame_buffer(void)
{
    return g_graphics_object_frame_buffer;
}

/************************************* SDL2 *************************************/

AR_API void ar_graphics_set_sdl_objects(SDL_Window* window, SDL_Renderer* renderer)
{
    g_graphics_object_window   = window;
    g_graphics_object_renderer = renderer;
}

AR_API SDL_Window* ar_graphics_get_sdl_window(void)
{
    return g_graphics_object_window;
}

AR_API SDL_Renderer* ar_graphics_get_sdl_renderer(void)
{
    return g_graphics_object_renderer;
}

AR_API void ar_graphics_set_gl_context(open_gl_context context)
{
    g_graphics_object_gl_context = context;
}

AR_API open_gl_context ar_graphics_get_gl_context(void)
{
    return g_graphics_object_gl_context;
}
