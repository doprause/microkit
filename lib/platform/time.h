
#ifndef MICROKIT_PLATFORM_API_TIME_H
#define MICROKIT_PLATFORM_API_TIME_H

#include "libs/microkit/lib/core.h"
#include "libs/microkit/lib/types.h"
#include "microkit/config/microkit.h"
#include "microkit/config/time.h"

#if MICROKIT_IS_CONFIGURED(MICROKIT_CONFIG_USE_TIME)

#if MICROKIT_IS_CONFIGURED(MICROKIT_CONFIG_TIME_DELAY_MICROS)
void microkit_time_delay_micros(UInt32 micros);
#endif // MICROKIT_IS_CONFIGURED(MICROKIT_CONFIG_TIME_DELAY_MICROS)

void microkit_time_delay_millis(UInt32 millis);

#endif // MICROKIT_IS_CONFIGURED(MICROKIT_CONFIG_USE_TIME)

#endif // MICROKIT_PLATFORM_API_TIME_H
