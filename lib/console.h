#ifndef MICROKIT_CONSOLE_H
#define MICROKIT_CONSOLE_H

#include "libs/microkit/lib/core.h"
#include "microkit/config/microkit.h"

#if MICROKIT_IS_CONFIGURED(MICROKIT_CONFIG_USE_CONSOLE)

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

} MicrokitConsoleInterface;

#endif // MICROKIT_CONFIG_USE_CONSOLE

#endif // MICROKIT_CONSOLE_H
