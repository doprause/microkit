/**
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
 *                 oo                            dP       oo   dP
 *                                              88            88
 *      88d8b.d8b. dP .d8888b. 88d888b. .d8888b. 88  .dP  dP d8888P
 *      88'`88'`88 88 88'  `"" 88'  `88 88'  `88 88888"   88   88
 *      88  88  88 88 88.  ... 88       88.  .88 88  `8b. 88   88
 *      dP  dP  dP dP `88888P' dP       `88888P' dP   `YP dP   dP
 *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
 * @file assert.h
 * @author Dominik Prause (dominik@inspirent.io)
 * @version 0.1
 * @date 2025-04-17
 * @copyright Copyright (c) 2025 - All rights reserved.
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
 */
#ifndef MICROKIT_ASSERT_H
#define MICROKIT_ASSERT_H

#include "contracts/mapi/assert.h"
#include "types.h"

/**
 * @brief Asserts that the given expression is true.
 */
#define ASSERT(expression)                                                     \
   do {                                                                        \
      if (!(expression)) {                                                     \
         mapi_gpio_assertion();                                                \
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
         mapi_gpio_assertion();                                                \
      }                                                                        \
   } while (0)

/**
 * @brief Asserts that the given value is not a null-pointer.
 * @param pointer The pointer to check.
 */
#define ASSERT_NOT_NULL_POINTER(pointer)                                       \
   do {                                                                        \
      if ((pointer) == NULL) {                                                 \
         mapi_gpio_assertion();                                                \
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
