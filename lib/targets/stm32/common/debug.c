

#include "microkit/lib/debug.h"
#include "microkit/lib/config/os.h"
#include "microkit/lib/modules/console/console.h"

#include <stm32h533xx.h>
#include <stm32h5xx_hal.h>

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * @brief Enagages or disengages the debug LED.
 */
void debug_led(bool enable) {

   if (enable) {
      HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);
   } else {
      HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);
   }
}

#if defined(CONFIG_OS_CONSOLE_ENABLE)
/**
 * @brief Prints the given string to the default debug output.
 */
void debug_print(const char* fmt, ...) {

   char buffer[CONFIG_OS_DEBUG_PRINT_BUFFER_SIZE];

   va_list args;
   va_start(args, fmt);
   vsnprintf(buffer, CONFIG_OS_DEBUG_PRINT_BUFFER_SIZE, fmt, args);
   va_end(args);

   console_put(buffer);
}
#endif
