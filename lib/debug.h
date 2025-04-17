/**
 * @file debug.h
 * @author D. Prause
 * @brief Provides assert macros.
 * @version 0.1
 * @date 2024-11-28
 *
 * @copyright Copyright (c) 2024
 *
 */

#ifndef MICROKIT_DEBUG_H
#define MICROKIT_DEBUG_H

#include "types.h"

/**
 * @brief This function engages or disengages a LED.
 * @note This function must be implemented for each target platform.
 * @param enable If TRUE, the  LED is turned on. Otherwise, the LED is turned
 * off.
 */
extern void debug_led(bool enable);

/**
 * @brief This function prints a string to the default debug output (usually
 * UART1).
 * @note This function must be implemented for each target platform.
 * @param fmt The format string to be printed.
 * @param ... The arguments to be printed.
 */
extern void debug_print(const char* fmt, ...);

#define ON 1
#define OFF 0

/**
 * @brief Switch LED on or off.
 * @param enable If TRUE, the LED is turned on. Otherwise, the LED is turned
 * off.
 */
#define DEBUG_LED(enable)                                                      \
   do {                                                                        \
      debug_led(enable);                                                       \
   } while (0)

/**
 * @brief
 *
 */
#define DEBUG_PRINT(fmt, ...)                                                  \
   do {                                                                        \
      debug_print(fmt, ##__VA_ARGS__);                                         \
   } while (0)

#endif
