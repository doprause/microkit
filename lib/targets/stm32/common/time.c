/** - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
 * @file time.c
 * @author D. Prause
 * @brief STM32 implementation of the 'Time' platform API.
 * @version 0.1
 * @date 2025-05-13
 * @copyright Copyright (c) 2025
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#include "libs/microkit/lib/core.h"
#include "libs/microkit/lib/platform/time.h"
#include "microkit/config/microkit.h"
#include "microkit/config/time.h"

#include <stm32h5xx_hal.h>

#if MICROKIT_IS_CONFIGURED(MICROKIT_CONFIG_USE_TIME)

#if MICROKIT_IS_CONFIGURED(MICROKIT_CONFIG_TIME_DELAY_MICROS)
void microkit_time_delay_micros(UInt32 micros) {
   // TODO: To be implemented
}
#endif

void microkit_time_delay_millis(UInt32 millis) {
   HAL_Delay(millis);
}

#endif