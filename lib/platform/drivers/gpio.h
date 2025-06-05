/**
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
 * @file gpio.h
 * @author Dominik Prause
 * @version 0.1
 * @date 2025-06-04
 * @copyright Copyright (c) 2025 - All rights reserved.
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
 */
#pragma once
#include "libs/microkit/lib/core.h"

#if MICROKIT_IS_CONFIGURED(MICROKIT_CONFIG_USE_GPIO)

#include "libs/microkit/lib/gpio.h"
#include "microkit/config/gpio.h"

void microkit_gpio_init(void);
void microkit_gpio_start(const MicrokitUartDevice device, MicrokitUartConfig config);
void microkit_gpio_stop(const MicrokitUartDevice device);

#endif // MICROKIT_IS_CONFIGURED(MICROKIT_CONFIG_USE_GPIO)
