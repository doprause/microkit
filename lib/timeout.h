/** - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
 * @file timeout.h
 * @author D. Prause
 * @brief Timeout API.
 * @version 0.1
 * @date 2025-05-13
 * @copyright Copyright (c) 2025
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#ifndef MICROKIT_TIMEOUT_H
#define MICROKIT_TIMEOUT_H

#include "libs/microkit/lib/core.h"
#include "libs/microkit/lib/types.h"
#include "microkit/config/microkit.h"

#if MICROKIT_IS_CONFIGURED(MICROKIT_CONFIG_USE_TIMEOUT)

typedef struct {

   void (*set)(UInt32 timeout, void* target, void (*handler)(void*));

} MicrokitTimeoutInterface;

#endif // MICROKIT_CONFIG_USE_TIMEOUT
#endif // MICROKIT_TIMEOUT_H