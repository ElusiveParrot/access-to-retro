/**
 * @file definitions.h
 */

/** @defgroup group_unit_testing_definitions Definitions
 *  Contains basic definitions for unit testing
 *  @{
 */

#ifndef ACCESS_TO_RETRO_DEFINITIONS_H
#define ACCESS_TO_RETRO_DEFINITIONS_H

#include <string.h>

/**
 * @brief Define a test
 * @param FN_NAME name of the testing function (not to be confused with function being tested)
 */
#define DEFINE_TEST(FN_NAME) int FN_NAME(void)

/**
 * @brief To be used when the test is finished (either success or failure)
 * @param CODE error/success code, use ERROR or SUCCESS macro
 */
#define COMPLETE_TEST(CODE) return CODE;

/**
 * @brief Indicates an error, to be used in COMPLETE_TEST macro
 * @param CODE error code, if zero is provided then -2 is used instead
 */
#define ERROR(CODE) (CODE == 0 ? -2 : CODE)

/// @brief Indicated no error happened, to be used in COMPLETE_TEST macro
#define SUCCESS 0

/// @brief Used to define entry point of test driver, to be used instead of main function
#define DEFINE_TESTING_ENTRY_POINT int main(int argc, char** argv)

/// @brief To be used at the beginning of 'DEFINE_STARTING_ENTRY_POINT'
#define START_TESTING (void)argc;

/// @brief To be used at the end of 'DEFINE_STARTING_ENTRY_POINT'
#define END_TESTING return -1;

/**
 * @brief Connect testing function with a name used in CMakeLists.txt
 * @param TEST_NAME Name used in CMakeLists.txt
 * @param TEST_FN Testing function
 */
#define DEFINE_TEST_FN(TEST_FN_NAME) \
if (strcmp(argv[1], #TEST_FN_NAME) == 0) \
{ \
    return TEST_FN_NAME(); \
} \

#endif //ACCESS_TO_RETRO_DEFINITIONS_H

/** @} */ // end of group
