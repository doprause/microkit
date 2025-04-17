/** - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
 * @file assert.h
 * @author D. Prause
 * @brief I2C HAL API.
 * @version 0.1
 * @date 2024-11-19
 * @copyright Copyright (c) 2025
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
#ifndef MICROKIT_ASSERT_H
#define MICROKIT_ASSERT_H

#include "microkit/lib/contracts/platform/assert.h"
#include "microkit/lib/types.h"

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

/**
 * @brief Asserts that the given expression is true.
 */
#define ASSERT(expression)                                                     \
   do {                                                                        \
      if (!(expression)) {                                                     \
         gpio_assertion();                                                     \
      }                                                                        \
   } while (0)

/**
 * @brief Asserts that the given expression is true.
 */
#define ASSERT_NOT(expression)                                                 \
   do {                                                                        \
      ASSERT(!(expression));                                                   \
   } while (0)

/**
 * @brief Asserts that the given value is a null-pointer.
 * @param pointer The pointer to check.
 */
#define ASSERT_NULL_POINTER(pointer)                                           \
   do {                                                                        \
      if ((pointer) != NULL) {                                                 \
         gpio_assertion();                                                     \
      }                                                                        \
   } while (0)

/**
 * @brief Asserts that the given value is not a null-pointer.
 * @param pointer The pointer to check.
 */
#define ASSERT_NOT_NULL_POINTER(pointer)                                       \
   do {                                                                        \
      if ((pointer) == NULL) {                                                 \
         gpio_assertion();                                                     \
      }                                                                        \
   } while (0)

/**
 * @brief Raises assertion when this code line is reached.
 */
#define ASSERT_NOT_REACHED                                                     \
   do {                                                                        \
      ASSERT(false);                                                           \
   } while (0)

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
#endif
