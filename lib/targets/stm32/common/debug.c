#include "libs/microkit/lib/core.h"
#include "libs/microkit/lib/debug.h"
#include "microkit/config/debug.h"
#include "microkit/config/microkit.h"
#include "libs/microkit/lib/microkit.h"

#include <stm32h533xx.h>
#include <stm32h5xx_hal.h>

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#if MICROKIT_IS_CONFIGURED(MICROKIT_CONFIG_USE_CONSOLE) && MICROKIT_IS_CONFIGURED(MICROKIT_CONFIG_USE_DEBUG_CONSOLE)

void debug_print(const char* fmt, ...) {

   char buffer[MICROKIT_CONFIG_DEBUG_PRINT_BUFFER_SIZE];

   va_list args;
   va_start(args, fmt);
   vsnprintf(buffer, MICROKIT_CONFIG_DEBUG_PRINT_BUFFER_SIZE, fmt, args);
   va_end(args);

   Microkit.console.write(buffer);
}

#endif

#if MICROKIT_IS_CONFIGURED(MICROKIT_CONFIG_USE_DEBUG_LED)

void debug_led(bool enable) {

   if (enable) {
      HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);
   } else {
      HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);
   }
}

#endif
