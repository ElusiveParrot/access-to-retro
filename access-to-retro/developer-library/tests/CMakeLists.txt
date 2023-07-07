include_directories(${SDL2_INCLUDE_DIRS})

# Data Tests
add_executable(ar_data_test
        data_tests.c
        ../src/data.c
        )

# Bit Manipulation
add_test(NAME data_set_bit_test COMMAND ar_data_test "ar_set_bit_test")
add_test(NAME data_clear_bit_test COMMAND ar_data_test "ar_clear_bit_test")
add_test(NAME data_set_bit_to_test COMMAND ar_data_test "ar_set_bit_to_test")
add_test(NAME data_toggle_bit_test COMMAND ar_data_test "ar_toggle_bit_test")
add_test(NAME data_is_bit_set_test COMMAND ar_data_test "ar_is_bit_set_test")

# Byte manipulation
add_test(NAME data_combine_n_bytes_test COMMAND ar_data_test "ar_combine_n_bytes_test")
