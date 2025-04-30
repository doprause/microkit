/**
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
 * @file microkit.c
 * @author Dominik Prause
 * @version 0.1
 * @date 2025-04-17
 * @copyright Copyright (c) 2025 - All rights reserved.
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
 */
#include "libs/microkit/lib/core.h"
#include "libs/microkit/lib/microkit.h"
#include "libs/microkit/lib/modules/console/console.h"
#include "libs/microkit/lib/platform/time.h"

static void init(void) {
}

static void start(void) {
}

static void stop(void) {
}

MicrokitInterface Microkit = {
    .init = init,
    .start = start,
    .stop = stop,

#if MICROKIT_IS_CONFIGURED(MICROKIT_CONFIG_USE_CONSOLE)
    .console = {
        .init = microkit_console_init,
        .start = microkit_console_start,
        .stop = microkit_console_stop,
        .read = microkit_console_read,
        .write = microkit_console_write,
        .writeLine = microkit_console_write_line},
#endif

#if MICROKIT_IS_CONFIGURED(MICROKIT_CONFIG_USE_TIME)
    .time = {.delay = {
#if MICROKIT_IS_CONFIGURED(MICROKIT_CONFIG_TIME_USE_DELAY_MICROS)
                 .micros = microkit_time_delay_micros,
#endif // MICROKIT_CONFIG_TIME_USE_DELAY_MICROS
                 .millis = microkit_time_delay_millis,
             }},
#endif // MICROKIT_CONFIG_USE_TIME
};
