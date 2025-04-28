
#include "microkit/lib/contracts/platform/time.h"
#include <stm32h5xx_hal.h>

void microkit_time_delay_ms(UInt32 milliseconds) {
   HAL_Delay(milliseconds);
}