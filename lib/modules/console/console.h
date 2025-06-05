/** - - - - - -- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
 *
 * @note This implementation is intended to get a very simple bi-directional
 * communication channel working. The console requires a transport driver (UART,
 * I2C, SPI, etc.) that implements the sysio.h interface. Most likey you want to
 * use the UART transport driver without interrupts to bring up a board.
 *
 * @example
 * char message[64];
 *
 * console_write("\r$ ");
 *  if (console_read(message, 64) > 0) {
 *      console_write(message);
 *      console_write("> OK\n");
 *  }
 * - - - - - -- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#ifndef MICROKIT_MODULES_CONSOLE_H
#define MICROKIT_MODULES_CONSOLE_H

#include "libs/microkit/lib/core.h"

#if MICROKIT_IS_CONFIGURED(MICROKIT_CONFIG_USE_CONSOLE)

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#include "libs/microkit/lib/types.h"
#include "microkit/config/console.h"

void microkit_console_init(void);
void microkit_console_start(void);
void microkit_console_stop(void);
int microkit_console_read(char* message, int maxLength);
void microkit_console_write(const char* message);
void microkit_console_write_line(const char* message);

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#endif // MICROKIT_IS_CONFIGURED(MICROKIT_CONFIG_USE_CONSOLE)
#endif // MICROKIT_MODULES_CONSOLE_H
