#include <access-to-retro-dev/access-to-retro-dev.h>
#include <stdlib.h>

/****************************************************************************************************
 API global objects
****************************************************************************************************/

/// @brief Global executable object
static struct ar_executable* g_executable = NULL;

/****************************************************************************************************
 API Implementation
****************************************************************************************************/

AR_API void ar_create_executable(const ar_byte* bytes, size_t size)
{
    g_executable = malloc(sizeof(struct ar_executable));
    g_executable->raw_bytes = bytes;
    g_executable->size      = size;
}

AR_API struct ar_executable* ar_get_executable(void)
{
    return g_executable;
}

AR_API void ar_free_executable(void)
{
    // Bytes are not owned by the library so don't delete g_executable->raw_bytes
    free(g_executable);
}
