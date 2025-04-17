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
 * console_put("\r$ ");
 *  if (console_get(message, 64) > 0) {
 *      console_put(message);
 *      console_put("> OK\n");
 *  }
 * - - - - - -- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#ifndef MICROKIT_CONSOLE_H
#define MICROKIT_CONSOLE_H

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

/**
 * @brief Initializes the console.
 */
void console_init();

void console_start();

void console_stop();

void console_log(char* message);

void console_warn(char* message);

void console_error(char* message);

/**
 * @brief Gets a message with maxLength from the console.
 *
 * @param message The received message.
 * @param maxLength The maximum length of the message.
 * @return The number of received characters.
 */
int console_get(char* message, int maxLength);

/**
 * @brief Puts a message to the console.
 *
 * @param message The message to be sent.
 */
void console_put(char* message);

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
#endif
