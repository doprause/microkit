/** - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
 * @file timeout.h
 * @author D. Prause
 * @brief Timeout platform API.
 * @version 0.1
 * @date 2025-05-13
 * @copyright Copyright (c) 2025
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#ifndef MICROKIT_PLATFORM_TIMEOUT_H
#define MICROKIT_PLATFORM_TIMEOUT_H

#include "libs/microkit/lib/timeout.h"

void microkit_timeout_init(void);

void microkit_timeout_start(void);

void microkit_timeout_stop(void);

void microkit_timeout_process(void);

void microkit_timeout_set(UInt16 timeout, void* target, void (*handler)(void*));

#endif // MICROKIT_PLATFORM_TIMEOUT_H