/**
 * @file asserts.h
 */

/** @defgroup group_asserts Asserts
 *  Contains asserting macros
 *  @{
 */

#ifndef ACCESS_TO_RETRO_ASSERTS_H
#define ACCESS_TO_RETRO_ASSERTS_H

#include <stdbool.h>
#include "definitions.h"

/**
 * @brief Assert that object is NULL
 * @remarks Uses '==' for equality check
 * @param OBJ Value to check
 * @param FAIL_CODE Error code returned from the test on failure
 */
#define ASSERT_NULL(OBJ, FAIL_CODE) \
if (OBJ != NULL) \
{ \
    COMPLETE_TEST(ERROR(FAIL_CODE)); \
} \

/**
 * @brief Assert that object is NOT NULL
 * @remarks Uses '!=' for not equality check
 * @param OBJ Value to check
 * @param FAIL_CODE Error code returned from the test on failure
 */
#define ASSERT_NOT_NULL(OBJ, FAIL_CODE) \
if (OBJ == NULL) \
{ \
    COMPLETE_TEST(ERROR(FAIL_CODE)); \
} \

/**
 * @brief Assert that two objects are equal
 * @remarks Uses '==' for equality check
 * @param OBJ1 First value
 * @param OBJ2 Second value
 * @param FAIL_CODE Error code returned from the test on failure
 */
#define ASSERT_EQ(OBJ1, OBJ2, FAIL_CODE) \
if (OBJ1 != OBJ2) \
{ \
    COMPLETE_TEST(ERROR(FAIL_CODE)); \
} \

/**
 * @brief Assert that value is true
 * @param OBJ Value to check
 * @param FAIL_CODE Error code returned from the test on failure
 */
#define ASSERT_TRUE(OBJ, FAIL_CODE) \
if (!OBJ) \
{ \
    COMPLETE_TEST(ERROR(FAIL_CODE)); \
} \

/**
 * @brief Assert that value is false
 * @param OBJ Value to check
 * @param FAIL_CODE Error code returned from the test on failure
 */
#define ASSERT_FALSE(OBJ, FAIL_CODE) \
if (OBJ) \
{ \
    COMPLETE_TEST(ERROR(FAIL_CODE)); \
} \

/**
 * @brief Assert that two objects are NOT equal
 * @remarks Uses '!=' for not equality check
 * @param OBJ1 First value
 * @param OBJ2 Second value
 * @param FAIL_CODE Error code returned from the test on failure
 */
#define ASSERT_NEQ(OBJ1, OBJ2, FAIL_CODE) \
if (OBJ1 == OBJ2) \
{ \
    COMPLETE_TEST(ERROR(FAIL_CODE)); \
} \

/**
 * @brief Assert that two strings are equal
 * @remarks Uses 'strcmp' for equality check
 * @param STR1 First string
 * @param STR2 Second string
 * @param FAIL_CODE Error code returned from the test on failure
 */
#define ASSERT_STR_EQ(STR1, STR2, FAIL_CODE) \
if (strcmp(STR1, STR2) != 0) \
{ \
    COMPLETE_TEST(ERROR(FAIL_CODE)); \
} \

/**
 * @brief Assert that two strings are NOT equal
 * @remarks Uses 'strcmp' for not equality check
 * @param OBJ1 First string
 * @param OBJ2 Second string
 * @param FAIL_CODE Error code returned from the test on failure
 */
#define ASSERT_STR_NEQ(OBJ1, OBJ2, FAIL_CODE) \
if (strcmp(STR1, STR2) == 0) \
{ \
    COMPLETE_TEST(ERROR(FAIL_CODE)); \
} \

/**
 * @brief Assert that other test did not fail
 * @param TEST_FN Function name of the other test
 * @param FAIL_CODE Error code returned from the test on failure
 */
#define ASSERT_TEST_SUCCEEDS(TEST_FN, FAIL_CODE) \
if (TEST_FN() != 0)\
{ \
    COMPLETE_TEST(ERROR(FAIL_CODE)); \
} \

/**
 * @brief Assert that other test did fail
 * @param TEST_FN Function name of the other test
 * @param FAIL_CODE Error code returned from the test on failure
 */
#define ASSERT_TEST_FAILS(TEST_FN, FAIL_CODE) \
if (TEST_FN() == 0)\
{ \
    COMPLETE_TEST(ERROR(FAIL_CODE)); \
} \

#endif //ACCESS_TO_RETRO_ASSERTS_H

/** @} */ // end of group
