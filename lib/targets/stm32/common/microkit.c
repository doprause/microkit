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
#include "libs/microkit/lib/modules/logger/logger.h"
#include "libs/microkit/lib/platform/drivers/uart.h"
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

    .driver = {
#if MICROKIT_IS_CONFIGURED(MICROKIT_CONFIG_USE_UART)
        .uart = {
            .init = microkit_uart_init,
            .start = microkit_uart_start,
            .stop = microkit_uart_stop,
            .receive = microkit_uart_receive,
            .send = microkit_uart_send,
        },
#endif
    },

#if MICROKIT_IS_CONFIGURED(MICROKIT_CONFIG_USE_CONSOLE)
    .console = {.init = microkit_console_init, .start = microkit_console_start, .stop = microkit_console_stop, .read = microkit_console_read, .write = microkit_console_write, .writeLine = microkit_console_write_line},
#endif

#if MICROKIT_IS_CONFIGURED(MICROKIT_CONFIG_USE_LOGGER)
    .logger = {
        .init = microkit_logger_init,
        .start = microkit_logger_start,
        .stop = microkit_logger_stop,
        .log = microkit_logger_log,
        .getLevel = microkit_logger_get_level,
        .setLevel = microkit_logger_set_level,
    },
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
