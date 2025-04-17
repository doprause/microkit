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
 * @file debug.h
 * @author Dominik Prause (dominik@inspirent.io)
 * @version 0.1
 * @date 2025-04-17
 * @copyright Copyright (c) 2025 - All rights reserved.
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
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
