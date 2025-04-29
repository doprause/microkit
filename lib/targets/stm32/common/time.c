
#include "microkit/lib/contracts/platform/time.h"
#include <stm32h5xx_hal.h>

void microkit_time_delay_micros(UInt32 micros) {
   HAL_Delay(micros); // TODO: Dummy implementation
}

void microkit_time_delay_millis(UInt32 millis) {
   HAL_Delay(millis);
}