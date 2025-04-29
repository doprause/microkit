/**
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
 * @file shell.h
 * @author Dominik Prause
 * @version 0.1
 * @date 2025-04-17
 * @copyright Copyright (c) 2025 - All rights reserved.
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
 */
#ifndef MICROKIT_SHELL_H
#define MICROKIT_SHELL_H

// #include "framework/software_timer.h"

#include "libs/microkit/lib/core.h"
#include "libs/microkit/lib/platform/drivers/uart.h"
#include "microkit/config/shell.h"
// #include "platform/api/serial.h"

#if MICROKIT_IS_CONFIGURED(MICROKIT_CONFIG_USE_SHELL)

#ifndef CLI_MAX_BUFFER_SIZE
#define CLI_MAX_BUFFER_SIZE 128
#endif

#ifndef CLI_ARGC_MAX_COUNT
#define CLI_ARGC_MAX_COUNT 8
#endif

#ifndef CLI_MAX_RESPONSE_LENGTH
#define CLI_MAX_RESPONSE_LENGTH 256
#endif

typedef struct ShellObject* ShellModule;

/**
 * @brief Macro to define a command.
 * @param commandName The command name (string).
 * @param handlerFunction The command handler (function pointer).
 * @param descriptionText The command description (string).
 */
#define COMMAND(commandName, handlerFunction, descriptionText) {.command = commandName, .handler = handlerFunction, .description = descriptionText}

/**
 * @brief Command handler function declaration.
 * @param argc The number of arguments.
 * @param argv The arguments.
 * @param cli A reference to the shell module instance.
 */
typedef Status (*CommandHandlerFunction)(int argc, char** argv, const ShellModule shell);

/**
 * @brief Command structure, consisting of a command name and a function
 * pointer to a command handler function.
 */
typedef struct {
   const char* command;
   CommandHandlerFunction handler;
   char* description;
} ShellCommand;

/**
 * @brief Macro to define a logger.
 * @param handlerFunction The logger handler (function pointer).
 */
#define LOGGER(loggerName, handlerFunction, disableFunction, statusFunction) {.logger = loggerName, .handler = handlerFunction, .disable = disableFunction, .status = statusFunction}

/**
 * @brief Logger disable function declaration.
 * @param shell A reference to the shell module instance.
 */
typedef Status (*LoggerDisableFunction)(const ShellModule shell);

/**
 * @brief Logger handler function declaration.
 * @param shell A reference to the shell module instance.
 */
typedef Status (*LoggerHandlerFunction)(const ShellModule shell);

/**
 * @brief Logger status function declaration.
 * @param shell A reference to the shell module instance.
 */
typedef bool (*LoggerStatusFunction)(const ShellModule shell);

/**
 * @brief Logger structure, consisting of a logger handler function.
 * @param loggerName The logger name (string).
 * @param handlerFunction The logger handler (function pointer).
 */
typedef struct {
   char* logger;
   LoggerDisableFunction disable;
   LoggerHandlerFunction handler;
   LoggerStatusFunction status;
} ShellLogger;

/**
 * @brief Shell interface.
 */
typedef struct {
   /**
    * @brief Initializes the given shell module instance.
    * @param softwareTimer The software timer to be used.
    * @param serial The serial communication interface to be used with the shell.
    * @param terminator The command terminator string.
    */
   void (*init)(
       const ShellModule shell,
       const UartDevice serial,
       const char* prompt,
       const char terminator);
   // ShellModule (*init)(
   //     const SoftwareTimerInstance softwareTimer,
   //     const SerialInstance serial,
   //     const char terminator,
   //     const SupervisorInstance supervisor,
   //     const PowerSimulatorInstance powerSimulator);

   // /**
   //  * @brief Destroys the instance and frees the memory.
   //  * @param instance The instance to destroy.
   //  */
   // void (*destroy)(const ShellModule shell);

   /**
    * @brief Starts the shell.
    * @param instance The instance.
    */
   void (*start)(const ShellModule shell);

   /**
    * @brief Stops the shell.
    * @param instance The instance.
    */
   void (*stop)(const ShellModule shell);

   /**
    * @brief Process commands if any.
    * This function must be called regulary in a superloop or a operating system task.
    * @param instance The instance to process.
    */
   void (*process)(const ShellModule shell);

   /**
    * @brief Writes the message to the shell output.
    * @param shell The shell.
    * @param message The message to be written.
    */
   void (*write)(const ShellModule shell, const char* message);

   // /**
   //  * @brief Gets the power simulator instance.
   //  * Typically used to get the power simulator instance within a command handler.
   //  * @param instance The instance.
   //  * @returns The power simulator instance.
   //  */
   // PowerSimulatorInstance (*powerSimulator)(const ShellModule shell);

   // /**
   //  * @brief Gets the supervisor instance.
   //  * Typically used to get the supervisor instance within a command handler.
   //  * @param instance The instance.
   //  * @returns The supervisor instance.
   //  */
   // SupervisorInstance (*supervisor)(const ShellModule shell);

} ShellInterface;

extern const ShellInterface Shell;

#endif // MICROKIT_CONFIG_USE_SHELL

#endif // MICROKIT_SHELL_H
