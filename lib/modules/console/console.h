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

#ifndef MKIT_CONSOLE_H
#define MKIT_CONSOLE_H

#include "microkit/lib/types.h"

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

/**
 * @brief Console interface.
 */
typedef struct {
   /**
    * @brief Initializes the console.
    */
   void (*init)(void);

   /**
    * @brief Starts the console.
    */
   void (*start)(void);

   /**
    * @brief Stops the console.
    */
   void (*stop)(void);

   /**
    * @brief Reads a message with maxLength from the console.
    * @param message The received message.
    * @param maxLength The maximum length of the message.
    * @return The number of received characters.
    */
   int (*read)(char* message, int maxLength);

   /**
    * @brief Writes the message to the console.
    * @param message The message to be sent.
    */
   void (*write)(const char* message);

   /**
    * @brief Writes a message and a line feed to the console.
    * @param message The message to be sent.
    */
   void (*writeLine)(const char* message);

} ConsoleInterface;

/**
 * @brief The singleton instance of the console.
 */
extern ConsoleInterface Console;

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
#endif
