#include <access-to-retro-dev/access-to-retro-dev.h>

/****************************************************************************************************
 API Implementation
****************************************************************************************************/

AR_API void ar_set_bit(ar_max_int* data, ar_bit_index bit_index)
{
    *data |= (ar_max_int) (1) << bit_index;
}

AR_API void ar_set_bit_to(ar_max_int* data, ar_bit_index bit_index, ar_bit new_value)
{
    if (new_value.value)
    {
        ar_set_bit(data, bit_index);
    }
    else
    {
        ar_clear_bit(data, bit_index);
    }
}

AR_API void ar_clear_bit(ar_max_int* data, ar_bit_index bit_index)
{
    *data &= ~((ar_max_int) (1) << bit_index);
}

AR_API void ar_toggle_bit(ar_max_int* data, ar_bit_index bit_index)
{
    *data ^= (ar_max_int) (1) << bit_index;
}

AR_API bool ar_is_bit_set(ar_max_int data, ar_bit_index bit_index)
{
    return (data >> bit_index) & (ar_max_int) (1);
}

AR_API ar_max_int ar_combine_n_bytes(const ar_byte* bytes, ar_max_int byte_count)
{
    ar_max_int constructed_int = 0;

    // Limit maximum bytes to 8, 64bit = 8 bytes
    for (ar_max_int i = 0; i < (byte_count > 8 ? 8 : byte_count); i++)
    {
        constructed_int |= ((ar_max_int) bytes[i] << (8 * i));
    }

    return constructed_int;
}
