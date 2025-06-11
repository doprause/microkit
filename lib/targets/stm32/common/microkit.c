/**
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
 * @file microkit.c
 * @author Dominik Prause
 * @version 0.1
 * @date 2025-04-17
 * @copyright Copyright (c) 2025 - All rights reserved.
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
 */
#include "libs/microkit/lib/assert.h"
#include "libs/microkit/lib/core.h"
#include "libs/microkit/lib/microkit.h"
#include "libs/microkit/lib/modules/console/console.h"
#include "libs/microkit/lib/modules/logger/logger.h"
#include "libs/microkit/lib/modules/shell/shell.h"
#include "libs/microkit/lib/platform/drivers/i2c.h"
#include "libs/microkit/lib/platform/drivers/i3c.h"
#include "libs/microkit/lib/platform/drivers/uart.h"
#include "libs/microkit/lib/platform/time.h"

#include "stm32h5xx_hal.h"

// extern void SystemClock_Config(void);
// extern void MX_GPIO_Init(void);
// extern void MX_ICACHE_Init(void);

static void init(InitCallback before, InitCallback after) {

   before();

   // Initialize drivers
#if MICROKIT_IS_CONFIGURED(MICROKIT_CONFIG_USE_I2C)
   Microkit.driver.i2c.init();
#endif

#if MICROKIT_IS_CONFIGURED(MICROKIT_CONFIG_USE_I3C)
   Microkit.driver.i3c.init();
#endif

#if MICROKIT_IS_CONFIGURED(MICROKIT_CONFIG_USE_UART)
   Microkit.driver.uart.init();
#endif

   // Initialize modules
#if MICROKIT_IS_CONFIGURED(MICROKIT_CONFIG_USE_CONSOLE)
   Microkit.console.init();
#endif

#if MICROKIT_IS_CONFIGURED(MICROKIT_CONFIG_USE_LOGGER)
   Microkit.logger.init();
#endif

#if MICROKIT_IS_CONFIGURED(MICROKIT_CONFIG_USE_SHELL)
   Microkit.shell.init();
#endif

   after();
}

static void start(void) {
   // Nothing to do right now
}

static void stop(void) {
   // Nothing to do right now
}

MicrokitInterface Microkit = {
    .init = init,
    .start = start,
    .stop = stop,

    .driver = {
#if MICROKIT_IS_CONFIGURED(MICROKIT_CONFIG_USE_GPIO)
        .gpio = {
            .init = microkit_gpio_init,
            .start = microkit_gpio_start,
            .stop = microkit_gpio_stop,
            .get = microkit_gpio_receive,
            .set = microkit_gpio_send,
            .toggle = microkit_gpio_toggle,
        },
#endif

#if MICROKIT_IS_CONFIGURED(MICROKIT_CONFIG_USE_I2C)
        .i2c = {
            .init = microkit_i2c_init,
            .start = microkit_i2c_start,
            .stop = microkit_i2c_stop,
            .receive = microkit_i2c_receive,
            .transmit = microkit_i2c_transmit,
            .memoryRead = microkit_i2c_memory_read,
            .memoryWrite = microkit_i2c_memory_write,
        },
#endif

#if MICROKIT_IS_CONFIGURED(MICROKIT_CONFIG_USE_I3C)
        .i3c = {
            .init = microkit_i3c_init,
            .start = microkit_i3c_start,
            .stop = microkit_i3c_stop,
            .process = microkit_i3c_process,
            .transmit = microkit_i3c_transmit,
            .memoryWrite = microkit_i3c_memory_write,
        },
#endif

#if MICROKIT_IS_CONFIGURED(MICROKIT_CONFIG_USE_UART)
        .uart = {
            .init = microkit_uart_init,
            .start = microkit_uart_start,
            .stop = microkit_uart_stop,
            .receive = microkit_uart_receive,
            .send = microkit_uart_send,
        },
#endif

    }, // driver

#if MICROKIT_IS_CONFIGURED(MICROKIT_CONFIG_USE_CONSOLE)
    .console = {
        .init = microkit_console_init,
        .start = microkit_console_start,
        .stop = microkit_console_stop,
        .read = microkit_console_read,
        .write = microkit_console_write,
        .writeLine = microkit_console_write_line,
    },
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

#if MICROKIT_IS_CONFIGURED(MICROKIT_CONFIG_USE_SHELL)
    .shell = {
        .init = microkit_shell_init,
        .start = microkit_shell_start,
        .stop = microkit_shell_stop,
        .process = microkit_shell_process,
        .write = microkit_shell_write,
        .writeError = microkit_shell_write_error,
        .writeLine = microkit_shell_write_line,
        .writeNewLine = microkit_shell_write_new_line,
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
