/**
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
 * @file assert.h
 * @author Dominik Prause
 * @version 0.1
 * @date 2025-04-17
 * @copyright Copyright (c) 2025 - All rights reserved.
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
 */
#pragma once

#include "libs/microkit/lib/core.h"
#include "libs/microkit/lib/platform/assert.h"
#include "libs/microkit/lib/types.h"
#include "microkit/config/microkit.h"

/**
 * @brief Asserts that the given expression is true.
 */
#if MICROKIT_IS_CONFIGURED(MICROKIT_CONFIG_USE_ASSERT)
#define ASSERT(expression) \
   do {                    \
      if (!(expression)) { \
         gpio_assertion(); \
      }                    \
   } while (0)
#else
#define ASSERT(expression)
#endif

/**
 * @brief Asserts that the given expression is true.
 */
#if MICROKIT_IS_CONFIGURED(MICROKIT_CONFIG_USE_ASSERT)
#define ASSERT_NOT(expression) \
   do {                        \
      ASSERT(!(expression));   \
   } while (0)
#else
#define ASSERT_NOT(expression)
#endif
/**
 * @brief Asserts that the given value is a null-pointer.
 * @param pointer The pointer to check.
 */
#if MICROKIT_IS_CONFIGURED(MICROKIT_CONFIG_USE_ASSERT)
#define ASSERT_NULL_POINTER(pointer) \
   do {                              \
      if ((pointer) != NULL) {       \
         gpio_assertion();           \
      }                              \
   } while (0)
#else
#define ASSERT_NULL_POINTER(expression)
#endif

/**
 * @brief Asserts that the given value is not a null-pointer.
 * @param pointer The pointer to check.
 */
#if MICROKIT_IS_CONFIGURED(MICROKIT_CONFIG_USE_ASSERT)
#define ASSERT_NOT_NULL_POINTER(pointer) \
   do {                                  \
      if ((pointer) == NULL) {           \
         gpio_assertion();               \
      }                                  \
   } while (0)
#else
#define ASSERT_NOT_NULL_POINTER(expression)
#endif

/**
 * @brief Raises assertion when this code line is reached.
 */
#if MICROKIT_IS_CONFIGURED(MICROKIT_CONFIG_USE_ASSERT)
#define ASSERT_NOT_REACHED \
   do {                    \
      ASSERT(false);       \
   } while (0)
#else
#define ASSERT_NOT_REACHED(expression)
#endif

