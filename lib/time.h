/** - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
 * @file time.h
 * @author D. Prause
 * @brief Time API.
 * @version 0.1
 * @date 2025-05-13
 * @copyright Copyright (c) 2025
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#ifndef MICROKIT_TIME_H
#define MICROKIT_TIME_H

#include "libs/microkit/lib/core.h"
#include "libs/microkit/lib/types.h"
#include "microkit/config/microkit.h"

#if MICROKIT_IS_CONFIGURED(MICROKIT_CONFIG_USE_TIME)

typedef struct {
   /**
    * @brief Busy wait for the specified number of microseconds.
    * @param micros Delay in microseconds.
    */
   void (*micros)(UInt32 micros);

   /**
    * @brief Busy wait for the specified number of milliseconds.
    * @param millis Delay in milliseconds.
    */
   void (*millis)(UInt32 millis);
} MicrokitTimeDelayInterface;

typedef struct {
   const MicrokitTimeDelayInterface delay;
} MicrokitTimeInterface;

#endif // MICROKIT_CONFIG_USE_TIME
#endif // MICROKIT_TIME_H