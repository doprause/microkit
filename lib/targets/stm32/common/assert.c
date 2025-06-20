/**
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
 * @file assert.c
 * @author Dominik Prause
 * @version 0.1
 * @date 2025-04-17
 * @copyright Copyright (c) 2025 - All rights reserved.
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
 */

#include "libs/microkit/lib/core.h"
#include "microkit/config/microkit.h"

#if MICROKIT_IS_CONFIGURED(MICROKIT_CONFIG_USE_ASSERT)

#include "libs/microkit/lib/platform/assert.h"
#include "libs/microkit/lib/targets/stm32/target.h"

#include <stm32h523xx.h>
#include <stm32h5xx_hal.h>

void gpio_assertion(void) {

   while (1) {
   }
}

#endif // MICROKIT_IS_CONFIGURED(MICROKIT_CONFIG_USE_ASSERT)
