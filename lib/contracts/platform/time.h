
#ifndef MICROKIT_TIME_H
#define MICROKIT_TIME_H

#include "microkit/lib/types.h"

/**
 * @brief Gets the number of milliseconds since the device booted.
 * @returns The number of milliseconds since the device booted.
 */
UInt64 microkit_time_ms_since_boot(void);

void microkit_time_delay_ms(UInt32 milliseconds);

#endif