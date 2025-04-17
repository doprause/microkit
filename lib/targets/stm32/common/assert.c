

#include "microkit/lib/contracts/platform/assert.h"

#include <stm32h533xx.h>
#include <stm32h5xx_hal.h>

void gpio_assertion(void) {

   // Enables LD2 on the Nucleo board
   HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);

   while (1) {
   }
}
