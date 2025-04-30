#include "shell.h"

#include "libs/microkit/lib/assert.h"
#include "libs/microkit/lib/microkit.h"
#include "libs/microkit/lib/utils/termcolor.h"
#include "libs/microkit/lib/platform/stdio.h"

#include <stdlib.h>
#include <string.h>

extern ShellCommand COMMANDS[];
extern Size COMMAND_COUNT;

extern ShellLogger LOGGERS[];
extern Size LOGGER_COUNT;

static const int STATUS_ERROR_UNKNOWN_COMMAND = -1000;

static const char* COMMAND_PROMPT = "$ ";                                               // colorize("host-emulator > ", TERMCOLOR_GREY)
static const char* COMMAND_FAILED = colorize("\nCommand failed\n", TERMCOLOR_RED);      // "\x1b[91mCommand failed\x1b[0m\r\n";
static const char* COMMAND_UNKNOWN = colorize("\nCommand unknown\n", TERMCOLOR_YELLOW); // "\x1b[36mCommand unknown\x1b[0m\r\n";

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
struct ShellObject {
   ShellCommand* commands;
   Size commandCount;
   bool commandPending;
   char* commandPrompt;
   ShellLogger* loggers;
   bool loggerActive;
   Size loggerCount;
   bool loggerPending;
   int loggerTimerId;
   char commandBuffer[CLI_MAX_BUFFER_SIZE];
   char receiveBuffer[CLI_MAX_BUFFER_SIZE];
   char* receiveBufferPointer;
   // PowerSimulatorInstance powerSimulator;
   UartDevice serial;
   // SupervisorInstance supervisor;
   // SoftwareTimerInstance softwareTimer;
   const char* prompt;
   char terminator;
};

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
struct ShellObject SHELL_OBJECT;
const ShellModule SHELL = &SHELL_OBJECT;

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
static Status private_interpret(const ShellModule instance) {

   UInt8 argc = 0;
   char* argv[CLI_ARGC_MAX_COUNT];

   // Get the first token (cmd name)
   argv[argc] = strtok(instance->commandBuffer, " ");

   // Return immediately if the first token is NULL
   if (argv[argc] == NULL) {
      return STATUS_OK;
   }

   // Walk through the other tokens (parameters)
   while ((argv[argc] != NULL) && (argc < CLI_ARGC_MAX_COUNT)) {
      argv[++argc] = strtok(NULL, " ");
   }

   // Search the command table for a matching command
   for (Size i = 0; i < instance->commandCount; i++) {
      if (strcmp(argv[0], instance->commands[i].command) == 0) {
         // Found a match, execute the associated function.
         return instance->commands[i].handler(argc, argv, instance);
      }
   }

   return STATUS_ERROR_UNKNOWN_COMMAND;
}


/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
static void private_print(const ShellModule shell, const char* message) {

   // Uart.send(shell->serial, (UInt8*)message, strlen(message)); // TODO: This doesn't work for some reason

   while (*message != '\0') {
      if (*message == '\r') {
         mkit_platform_stdio_put('\n');
      } else {
         mkit_platform_stdio_put(*message);
      }
      message++;
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
static void init(
    const ShellModule instance,
    //  const SoftwareTimerInstance softwareTimer,
    const UartDevice serial,
    const char* prompt,
    const char terminator
    //  const SupervisorInstance supervisor,
    //  const PowerSimulatorInstance powerSimulator
) {

   ASSERT_NOT_NULL_POINTER(instance);

   instance->commands = COMMANDS;
   instance->commandCount = COMMAND_COUNT;
   instance->commandPending = false;
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
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
static void stop(const ShellModule instance) {

   ASSERT_NOT_NULL_POINTER(instance);

   // SoftwareTimer.stop(instance->softwareTimer, instance->loggerTimerId);
   Microkit.driver.uart.stop(instance->serial);
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
static void start(const ShellModule instance) {

   ASSERT_NOT_NULL_POINTER(instance);

   UartConfig config = {
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

   private_print(instance, instance->prompt);
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
static void process_character(const ShellModule instance, Int32 character) {

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
         instance->commandPending = true;
      }
   } else if (character == '\b') {
      // Backspace. Delete character.
      if (instance->receiveBufferPointer > instance->receiveBuffer) {
         instance->receiveBufferPointer--;
      }
   } else {
      // Normal character received, add to buffer
      if ((instance->receiveBufferPointer - instance->receiveBuffer) < CLI_MAX_BUFFER_SIZE) {
         *(instance->receiveBufferPointer) = character;
         instance->receiveBufferPointer++;
      }
      // Echo the received character to console
      const char echo[] = {character, '\0'};
      private_print(instance, echo);
   }
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
static void process(const ShellModule instance) {

   ASSERT_NOT_NULL_POINTER(instance);

   UInt8 character;
   Status uartRxStatus = Microkit.driver.uart.receive(instance->serial, &character, 1);

   while (uartRxStatus == STATUS_OK && !instance->commandPending) {
      process_character(instance, character);
      uartRxStatus = Microkit.driver.uart.receive(instance->serial, &character, 1);
   }

   // Process the next command, if any
   // commandPending is set when a terminator was detected
   if (instance->commandPending) {
      Status status = private_interpret(instance);

      if (status == STATUS_ERROR) {
         private_print(instance, COMMAND_FAILED);
      } else if (status == STATUS_ERROR_UNKNOWN_COMMAND) {
         private_print(instance, COMMAND_UNKNOWN);
      }

      private_print(instance, instance->prompt);

      instance->commandPending = false;
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
void write(const ShellModule instance, const char* message) {

   ASSERT_NOT_NULL_POINTER(instance);

   private_print(instance, message);
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

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
const ShellInterface Shell = {
    .init = init,
    .start = start,
    .stop = stop,
    .process = process,
    .write = write,
    //  .powerSimulator = powerSimulator,
    //  .supervisor = supervisor,
};
