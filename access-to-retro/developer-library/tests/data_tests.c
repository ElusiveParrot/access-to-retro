#include <stdio.h>
#include "../include/access-to-retro-dev/access-to-retro-dev.h"
#include "../include/access-to-retro-dev/unit-testing-library/access-to-retro-unit-testing.h"

DEFINE_TEST(ar_set_bit_test)
{
    // Binary: 0000000000001010
    ar_max_int value = 10;

    // Set the bit at index 0 making: 0000000000001011 (17)
    ar_set_bit(&value, 0);

    ASSERT_EQ(value, 11, ERROR(1));

    COMPLETE_TEST(SUCCESS);
}

DEFINE_TEST(ar_clear_bit_test)
{
    // Binary: 0000000000001010
    ar_max_int value = 10;

    // Clear the bit at index 1 making: 0000000000001000 (8)
    ar_clear_bit(&value, 1);

    ASSERT_EQ(value, 8, ERROR(1));

    COMPLETE_TEST(SUCCESS);
}

DEFINE_TEST(ar_set_bit_to_test)
{
    // If setting and clearing a bit works then this works too
    ASSERT_TEST_SUCCEEDS(ar_clear_bit_test, ERROR(1))
    ASSERT_TEST_SUCCEEDS(ar_set_bit_test, ERROR(2))

    COMPLETE_TEST(SUCCESS)
}

DEFINE_TEST(ar_toggle_bit_test)
{
    // Binary: 0000000000001010
    ar_max_int value = 10;

    // Toggle from 0 to 1:

    // Toggle the bit at index 0 making: 0000000000001010 (11)
    ar_toggle_bit(&value, 0);

    ASSERT_EQ(value, 11, ERROR(1));

    // Toggle from 1 to 0:

    // Toggle the bit at index 0 making: 0000000000001010 (10)
    ar_toggle_bit(&value, 0);

    ASSERT_EQ(value, 10, ERROR(1));

    COMPLETE_TEST(SUCCESS);
}

DEFINE_TEST(ar_is_bit_set_test)
{
    // Binary: 0000000000001010
    ar_max_int value = 10;

    // Get the bit at index 0 (0)
    ASSERT_FALSE(ar_is_bit_set(value, 0), ERROR(1));

    // Get the bit at index 1 (1)
    ASSERT_TRUE(ar_is_bit_set(value, 1), ERROR(1));

    COMPLETE_TEST(SUCCESS);
}

DEFINE_TEST(ar_combine_n_bytes_test)
{
    ar_byte bytes[8] = { 0xAB, 0xCD, 0xEF, 0xFF, 0xFF, 0xEF, 0xCD, 0xAB };

    ar_max_int res = ar_combine_n_bytes(bytes, 8);

    printf("%lx", res);

    ASSERT_EQ(res, 0xabcdefffffefcdab, ERROR(1));

    COMPLETE_TEST(SUCCESS)
}

DEFINE_TESTING_ENTRY_POINT
{
    START_TESTING

    // Bit manipulation
    DEFINE_TEST_FN(ar_set_bit_test)
    DEFINE_TEST_FN(ar_clear_bit_test)
    DEFINE_TEST_FN(ar_set_bit_to_test)
    DEFINE_TEST_FN(ar_toggle_bit_test)
    DEFINE_TEST_FN(ar_is_bit_set_test)

    // Byte manipulation
    DEFINE_TEST_FN(ar_combine_n_bytes_test)

    END_TESTING
}
