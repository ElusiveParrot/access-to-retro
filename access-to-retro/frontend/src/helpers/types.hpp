/**
 * @file helpers/types.hpp
 */

#ifndef ACCESS_TO_RETRO_TYPES_HPP
#define ACCESS_TO_RETRO_TYPES_HPP

#include <cstdint>

/// @brief Contains typedefs
namespace ar::types
{
    /// @brief C-like string, mostly used for dealing with Access to Retro Library C API
    typedef const char* c_str;

    /// @brief C-like Pointer to generic memory, mostly used for dealing with Access to Retro Library C API
    typedef void* c_generic_memory_ptr;

    /// @brief Error code, mostly used for dealing with Access to Retro Library C API
    typedef uint32_t err_code;

    /// @brief Represents a single byte
    typedef uint8_t byte;
}

#endif //ACCESS_TO_RETRO_TYPES_HPP
