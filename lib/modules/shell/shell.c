#include "libs/microkit/lib/assert.h"
#include "libs/microkit/lib/core.h"
#include "libs/microkit/lib/microkit.h"
#include "libs/microkit/lib/platform/stdio.h"
#include "libs/microkit/lib/utils/termcolor.h"
#include "shell.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#if MICROKIT_IS_CONFIGURED(MICROKIT_CONFIG_USE_SHELL)

extern ShellCommand COMMANDS[];
extern Size COMMAND_COUNT;

extern ShellLogger LOGGERS[];
extern Size LOGGER_COUNT;

static const int STATUS_ERROR_UNKNOWN_COMMAND = -1000;

static const char* COMMAND_PROMPT = "$ ";                                               // colorize("host-emulator > ", TERMCOLOR_GREY)
static const char* COMMAND_FAILED = colorize("\nCommand failed\n", TERMCOLOR_RED);      // "\x1b[91mCommand failed\x1b[0m\r\n";
static const char* COMMAND_UNKNOWN = colorize("\nUnknown command\n", TERMCOLOR_YELLOW); // "\x1b[36mCommand unknown\x1b[0m\r\n";

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
typedef struct {
   char history[MICROKIT_SHELL_COMMAND_HISTORY_SIZE][MICROKIT_SHELL_MAX_COMMAND_SIZE];
   int currentIndex;
   int currentPosition;
   int historyCount;
} MicrokitShellCommandHistory;

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
struct MicrokitShellObject {
   ModuleState moduleState;
   ShellCommand* commands;
   Size commandCount;
   bool commandPending;
   char* commandPrompt;
   bool arrowKeyPressed;
   bool errorPrinted;
   bool escapeSequence;
   ShellLogger* loggers;
   bool loggerActive;
   Size loggerCount;
   bool loggerPending;
   int loggerTimerId;
   char commandBuffer[MICROKIT_SHELL_MAX_COMMAND_SIZE];
   MicrokitShellCommandHistory commandHistory;
   char receiveBuffer[MICROKIT_SHELL_MAX_COMMAND_SIZE];
   char* receiveBufferPointer;
   // PowerSimulatorInstance powerSimulator;
   MicrokitUartDevice serial;
   // SupervisorInstance supervisor;
   // SoftwareTimerInstance softwareTimer;
   const char* prompt;
   char terminator;
};

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
MicrokitShellCommandHistory SHELL_COMMAND_HISTORY = {
    .currentIndex = 0,
    .currentPosition = -1,
    .historyCount = 0,
};

struct MicrokitShellObject SHELL_OBJECT = {
    .moduleState = MKIT_MODULE_STATE_UNINITIALIZED,
};
const ShellModule SHELL = &SHELL_OBJECT;

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
static void private_print(const ShellModule shell, const char* output) {

   // Uart.send(shell->serial, (UInt8*)output, strlen(output)); // TODO: This doesn't work for some reason

   for (int i = 0; output[i] != '\0'; i++) {
      mkit_platform_stdio_put(output[i]);
   }
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
static void private_print_formatted(const ShellModule shell, const char* format, va_list args) {

   const Size MAX_WRITE_LENGTH = 128;
   char output[MAX_WRITE_LENGTH];

   vsnprintf(output, MAX_WRITE_LENGTH, format, args);

   private_print(shell, output);
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
void private_add_command_to_history(MicrokitShellCommandHistory* history, const char* command) {

   if (strlen(command) == 0)
      return; // Don't add empty commands

   int lastCommandIndex = (history->currentIndex == 0) ? (MICROKIT_SHELL_COMMAND_HISTORY_SIZE - 1) : (history->currentIndex - 1);

   if (history->historyCount > 0 && strncmp(history->history[lastCommandIndex], command, MICROKIT_SHELL_MAX_COMMAND_SIZE) == 0) {
      return; // Don't add the command if it's the same as the last one
   }

   strncpy(history->history[history->currentIndex], command, MICROKIT_SHELL_MAX_COMMAND_SIZE);

   history->currentIndex = (history->currentIndex + 1) % MICROKIT_SHELL_COMMAND_HISTORY_SIZE;

   if (history->historyCount < MICROKIT_SHELL_COMMAND_HISTORY_SIZE) {
      history->historyCount++;
   }

   history->currentPosition = history->currentIndex;
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
const char* private_get_command_from_history(MicrokitShellCommandHistory* history, int steps) {

   if (history->historyCount == 0) {
      return NULL;
   }

   history->currentPosition += steps;

   if (history->currentPosition < 0) {
      history->currentPosition = history->historyCount - 1;
   } else if (history->currentPosition >= history->historyCount) {
      history->currentPosition = 0;
   }

   return history->history[history->currentPosition];
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
static void private_print_command_from_history(const ShellModule instance, int steps) {

   const char* command = private_get_command_from_history(&instance->commandHistory, steps);

   if (command) {
      strcpy(instance->receiveBuffer, command);
      instance->receiveBufferPointer = instance->receiveBuffer + strlen(command);

      private_print(instance, "\r\033[K"); // Clear line
      private_print(instance, instance->prompt);
      private_print(instance, command);
   }
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
static Status private_interpret(const ShellModule instance) {

   UInt8 argc = 0;
   char* argv[MICROKIT_SHELL_ARGC_MAX_COUNT];
   char command[MICROKIT_SHELL_MAX_COMMAND_SIZE] = {0};

   // Get the first token (cmd name)
   argv[argc] = strtok(instance->commandBuffer, " ");

   // Return immediately if the first token is NULL
   if (argv[argc] == NULL) {
      return STATUS_OK;
   }

   strcat(command, argv[argc]);

   // Walk through the other tokens (parameters)
   while ((argv[argc] != NULL) && (argc < MICROKIT_SHELL_ARGC_MAX_COUNT)) {
      argv[++argc] = strtok(NULL, " ");

      if (argv[argc] != NULL) {
         strcat(command, " ");
         strcat(command, argv[argc]);
      }
   }

   private_add_command_to_history(&instance->commandHistory, command);

   // Search the command table for a matching command
   for (Size i = 0; i < instance->commandCount; i++) {
      if (strcmp(argv[0], instance->commands[i].command) == 0) {
         // Found a match and execute the associated function.
         return instance->commands[i].handler(argc, argv, instance);
      }
   }

   return STATUS_ERROR_UNKNOWN_COMMAND;
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
static void private_process_character(const ShellModule instance, Int32 character) {

   bool setEscapeSequence = false;

   if (character < 0) {
      return;
   }

   // Disable all loggers if logging is active
   if (instance->loggerActive) {
      for (Size i = 0; i < instance->loggerCount; i++) {
         instance->loggers[i].disable(instance);
      }

      instance->loggerActive = false;
      instance->loggerPending = false;

      // Flush input buffer
      UInt8 character;
      while (0 != Microkit.driver.uart.receive(instance->serial, &character, 1)) {
         // Do nothing
      };
   }

   if (character == instance->terminator) {
      // Message terminator.
      if (!instance->commandPending) {
         *(instance->receiveBufferPointer) = '\0';                 // Terminate the message
         strcpy(instance->commandBuffer, instance->receiveBuffer); // Copy message to command buffer

         instance->receiveBufferPointer = instance->receiveBuffer; // Reset receive buffer pointer to beginning

         if (strcmp(instance->commandBuffer, "") == 0) {
            private_print(instance, "\n");
         }

         instance->commandPending = true;
      }
   } else if (character == '\b' || character == 127) {
      // Backspace. Delete character.
      // BS (ASCII 8) or DEL (ASCII 127)
      if (instance->receiveBufferPointer > instance->receiveBuffer) {
         instance->receiveBufferPointer--;
         // Remove the last character from console
         private_print(instance, "\b \b");
      }
   } else if (character == '\033') {
      // Escape character
      setEscapeSequence = true;
   } else if (instance->escapeSequence && character == '[') {
      instance->arrowKeyPressed = true;
   } else if (instance->escapeSequence && instance->arrowKeyPressed && character == 'A') {
      private_print_command_from_history(instance, -1); // Arrow up
      instance->arrowKeyPressed = false;
   } else if (instance->escapeSequence && instance->arrowKeyPressed && character == 'B') {
      private_print_command_from_history(instance, 1); // Arrow down
      instance->arrowKeyPressed = false;
   } else {
      // Normal character received, add to buffer
      if ((instance->receiveBufferPointer - instance->receiveBuffer) < MICROKIT_SHELL_MAX_COMMAND_SIZE) {
         *(instance->receiveBufferPointer) = character;
         instance->receiveBufferPointer++;
      }
      // Echo the received character to console
      const char echo[] = {character, '\0'};
      private_print(instance, echo);
   }

   if (instance->escapeSequence && !instance->arrowKeyPressed) {
      instance->escapeSequence = false;
   }

   if (setEscapeSequence) {
      instance->escapeSequence = true;
   }
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
// static void onLoggerTimerElapsed(const SoftwareTimerInstance timer, void* target, uint32 time) {

//    UNUSED_ARG(timer);
//    UNUSED_ARG(time);

//    ShellModule instance = (ShellModule)target;

//    ASSERT_NOT_NULL_POINTER(instance);

//    instance->loggerPending = true;
// }

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
void microkit_shell_init(
    const ShellModule instance,
    //  const SoftwareTimerInstance softwareTimer,
    const MicrokitUartDevice serial,
    const char* prompt,
    const char terminator
    //  const SupervisorInstance supervisor,
    //  const PowerSimulatorInstance powerSimulator
) {

   ASSERT_NOT_NULL_POINTER(instance);
   ASSERT(instance->moduleState == MKIT_MODULE_STATE_UNINITIALIZED);

   instance->commands = COMMANDS;
   instance->commandCount = COMMAND_COUNT;
   instance->commandPending = false;
   instance->commandHistory.currentIndex = 0;
   instance->commandHistory.currentPosition = -1;
   instance->commandHistory.historyCount = 0;
   instance->arrowKeyPressed = false;
   instance->escapeSequence = false;
   instance->errorPrinted = false;
   instance->loggers = LOGGERS;
   instance->loggerActive = false;
   instance->loggerCount = LOGGER_COUNT;
   instance->loggerPending = false;
   instance->loggerTimerId = 0;
   instance->receiveBufferPointer = instance->receiveBuffer;
   instance->serial = serial;
   // instance->softwareTimer = softwareTimer;
   // instance->supervisor = supervisor;
   // instance->powerSimulator = powerSimulator;
   // volatile int test = strcmp(prompt, "123");
   instance->prompt = strcmp(prompt, "") != 0 ? prompt : COMMAND_PROMPT,
   instance->terminator = terminator;

   instance->moduleState = MKIT_MODULE_STATE_STOPPED;
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
void microkit_shell_start(const ShellModule instance) {

   ASSERT_NOT_NULL_POINTER(instance);
   ASSERT(instance->moduleState != MKIT_MODULE_STATE_UNINITIALIZED);

   MicrokitUartConfig config = {
       .baudrate = MKIT_UART_BAUDRATE_115200,
       .databits = MKIT_UART_DATABITS_8,
       .stopbits = MKIT_UART_STOPBITS_1,
       .parity = MKIT_UART_PARITY_NONE,
   };

   Microkit.driver.uart.start(instance->serial, config);

   // instance->loggerTimerId = SoftwareTimer.start(
   //     instance->softwareTimer,
   //     onLoggerTimerElapsed,
   //     instance,
   //     1000,
   //     TIMER_PERIODIC);

   instance->moduleState = MKIT_MODULE_STATE_RUNNING;

   private_print(instance, instance->prompt);
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
void microkit_shell_stop(const ShellModule instance) {

   ASSERT_NOT_NULL_POINTER(instance);
   ASSERT(instance->moduleState != MKIT_MODULE_STATE_UNINITIALIZED);

   // SoftwareTimer.stop(instance->softwareTimer, instance->loggerTimerId);
   Microkit.driver.uart.stop(instance->serial);

   instance->moduleState = MKIT_MODULE_STATE_STOPPED;
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
void microkit_shell_process(const ShellModule instance) {

   ASSERT_NOT_NULL_POINTER(instance);
   ASSERT(instance->moduleState == MKIT_MODULE_STATE_RUNNING);

   UInt8 character;
   Status uartRxStatus = Microkit.driver.uart.receive(instance->serial, &character, 1);

   while (uartRxStatus == STATUS_OK && !instance->commandPending) {
      private_process_character(instance, character);
      uartRxStatus = Microkit.driver.uart.receive(instance->serial, &character, 1);
   }

   // Process the next command, if any
   // commandPending is set when a terminator was detected
   if (instance->commandPending) {
      Status status = private_interpret(instance);

      if (status == STATUS_ERROR && !instance->errorPrinted) {
         private_print(instance, COMMAND_FAILED);
      } else if (status == STATUS_ERROR_UNKNOWN_COMMAND) {
         private_print(instance, COMMAND_UNKNOWN);
      }

      private_print(instance, instance->prompt);

      instance->commandPending = false;
      instance->errorPrinted = false;
   }

   // Process loggers
   // for (Size i = 0; i < instance->loggerCount; i++) {
   //    if (instance->loggers[i].status != NULL && instance->loggers[i].status(instance)) {
   //       instance->loggerActive = true;

   //       if (instance->loggerPending) {
   //          instance->loggers[i].handler(instance);
   //       }
   //    }
   // }

   // Reset the loggers pending flag
   // if (instance->loggerPending) {
   //    instance->loggerPending = false;
   // }
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
void microkit_shell_write(const ShellModule instance, const char* format, ...) {

   ASSERT_NOT_NULL_POINTER(instance);
   ASSERT(instance->moduleState == MKIT_MODULE_STATE_RUNNING);

   va_list args;
   va_start(args, format);
   private_print_formatted(instance, format, args);
   va_end(args);
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
void microkit_shell_write_error(const ShellModule instance, const char* format, ...) {
   // microkit_shell_write_new_line(instance, format);
   // microkit_shell_write_new_line(instance, colorize(format, TERMCOLOR_RED));

   ASSERT_NOT_NULL_POINTER(instance);
   ASSERT(instance->moduleState == MKIT_MODULE_STATE_RUNNING);

   va_list args;
   va_start(args, format);
   private_print(instance, "\n");
   private_print(instance, TERMCOLOR_RED);
   private_print(instance, "Error: ");
   private_print(instance, TERMCOLOR_RESET);
   private_print_formatted(instance, format, args);
   private_print(instance, "\n");
   va_end(args);

   instance->errorPrinted = true;
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
void microkit_shell_write_line(const ShellModule instance, const char* format, ...) {
   // microkit_shell_write(instance, format);
   // microkit_shell_write(instance, "\n");

   ASSERT_NOT_NULL_POINTER(instance);
   ASSERT(instance->moduleState == MKIT_MODULE_STATE_RUNNING);

   va_list args;
   va_start(args, format);
   private_print_formatted(instance, format, args);
   private_print(instance, "\n");
   va_end(args);
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
void microkit_shell_write_new_line(const ShellModule instance, const char* format, ...) {
   // microkit_shell_write(instance, "\n");
   // microkit_shell_write_line(instance, format);

   ASSERT_NOT_NULL_POINTER(instance);
   ASSERT(instance->moduleState == MKIT_MODULE_STATE_RUNNING);

   va_list args;
   va_start(args, format);
   private_print(instance, "\n");
   private_print_formatted(instance, format, args);
   private_print(instance, "\n");
   va_end(args);
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
// PowerSimulatorInstance powerSimulator(const ShellModule instance) {

//    ASSERT_NOT_NULL_POINTER(instance);

//    return instance->powerSimulator;
// }

// /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
// SupervisorInstance supervisor(const ShellModule instance) {

//    ASSERT_NOT_NULL_POINTER(instance);

//    return instance->supervisor;
// }

#endif // MICROKIT_IS_CONFIGURED(MICROKIT_CONFIG_USE_SHELL)
