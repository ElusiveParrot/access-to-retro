/**
 * @file graphics.h
 */

/** @defgroup group_data Data
 *  Helper functions dealing with data
 *  @{
 */

#ifndef ACCESS_TO_RETRO_DATA_H
#define ACCESS_TO_RETRO_DATA_H

#include <stdbool.h>
#include <stdint.h>
#include "basics.h"

/************************************* Data Types *************************************/

/**
 * @brief Type representing a single byte inside the memory
 * @remark This type is unsigned, for signed byte see 'ar_sbyte'
 */
typedef uint8_t ar_byte;

/**
 * @brief Type representing a single byte inside the memory
 * @remark This type is signed, for unsigned byte see 'ar_sbyte'
 */
typedef uint8_t ar_sbyte;

/// @brief Type representing generic memory pointer.
typedef void* ar_generic_memory_ptr;

/**
 * @brief Type representing widest possible integer for this platform
 * @details This is useful for functions that perform bitwise operations and allow integer of any size to be changed,
 *          this type helps with that as any integer type can be promoted to this one.
 * @remark This type is unsigned, for signed equivalent see 'ar_max_sint'
 */
typedef uintmax_t ar_max_int;

/**
 * @brief Type representing widest possible integer for this platform
 * @details This is useful for functions that perform bitwise operations and allow integer of any size to be changed,
 *          this type helps with that as any signed integer type can be promoted to this one.
 * @warning 'ar_max_int' is generally preferred and should always be used if promotion from integers larger than
 *          32-bit is possible
 * @remark This type is signed, for unsigned equivalent see 'ar_max_int'
 */
typedef intmax_t ar_max_sint;

/**
 * @brief Type representing a single bit
 * @remark While it represents a bit, it shouldn't be used to save memory as most compiler will still use a byte for
 *         this object - this structure is for convenience
 */
struct ar_bit
{
    /// @brief Bit value bitfield
    uint8_t value: 1;
};

/// @brief Typedef for 'ar_bit' structure to 'simulate' primitive type
typedef struct ar_bit ar_bit;

/// @brief Type representing an index of a bit inside a number
typedef uint8_t ar_bit_index;

/************************************* Bit Manipulation *************************************/

/**
 * @brief Set specific bit inside the number by setting it to '1'
 * @param data Number in which the bit will be set
 * @param bit_index Index of the bit, 0 indexed
 */
AR_API void ar_set_bit(ar_max_int* data, ar_bit_index bit_index);

/**
 * @brief Set specific bit inside the number to provided value
 * @param data Number in which the bit will be set
 * @param bit_index Index of the bit, 0 indexed
 * @param new_value Value to which the bit will be set
 */
AR_API void ar_set_bit_to(ar_max_int* data, ar_bit_index bit_index, ar_bit new_value);

/**
 * @brief Clear specific bit inside the number by setting it to '0'
 * @param data Number in which the bit will be cleared
 * @param bit_index Index of the bit, 0 indexed
 */
AR_API void ar_clear_bit(ar_max_int* data, ar_bit_index bit_index);

/**
 * @brief Toggles a specific bit inside the number
 * @details If bit is 0 then it will be set, if bit is 1 then it will be cleared
 * @param data Number in which the bit will change its value
 * @param bit_index Index of the bit, 0 indexed
 */
AR_API void ar_toggle_bit(ar_max_int* data, ar_bit_index bit_index);

/**
 * @brief Checks whether given bit inside the number is set (its value is '1')
 * @param data Number in which the bit will be checked
 * @param bit_index Index of the bit, 0 indexed
 * @return Whether the bit is set
 */
AR_API bool ar_is_bit_set(ar_max_int data, ar_bit_index bit_index);

/************************************* Byte Manipulation *************************************/

/**
 * @brief Combine n bytes into a single integer
 * @remarks Up to 8 bytes will be used, if more provided they will be ignored
 * @param bytes Array of bytes that will be used
 * @param byte_count Number of bytes provided
 * @return Constructed integer
 */
AR_API ar_max_int ar_combine_n_bytes(const ar_byte* bytes, ar_max_int byte_count);

#endif //ACCESS_TO_RETRO_DATA_H

/** @} */ // end of group
