#ifndef MICROKIT_H
#define MICROKIT_H

#include "libs/microkit/lib/console.h"
#include "libs/microkit/lib/core.h"
#include "libs/microkit/lib/i2c.h"
#include "libs/microkit/lib/logger.h"
#include "libs/microkit/lib/shell.h"
#include "libs/microkit/lib/time.h"
#include "libs/microkit/lib/timeout.h"
#include "libs/microkit/lib/uart.h"

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
   👉 Driver API
   - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
typedef struct {

#if MICROKIT_IS_CONFIGURED(MICROKIT_CONFIG_USE_I2C)
   MicrokitDriverI2cInterface i2c;
#endif

#if MICROKIT_IS_CONFIGURED(MICROKIT_CONFIG_USE_UART)
   MicrokitDriverUartInterface uart;
#endif

} MicrokitDriverInterface;

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
   👉 Microkit API
   - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
typedef struct {
   void (*init)(void);
   void (*start)(void);
   void (*stop)(void);

   MicrokitDriverInterface driver;

#if MICROKIT_IS_CONFIGURED(MICROKIT_CONFIG_USE_CONSOLE)
   MicrokitConsoleInterface console;
#endif

#if MICROKIT_IS_CONFIGURED(MICROKIT_CONFIG_USE_LOGGER)
   MicrokitLoggerInterface logger;
#endif

#if MICROKIT_IS_CONFIGURED(MICROKIT_CONFIG_USE_SHELL)
   MicrokitShellInterface shell;
#endif

#if MICROKIT_IS_CONFIGURED(MICROKIT_CONFIG_USE_TIME)
   MicrokitTimeInterface time;
#endif

#if MICROKIT_IS_CONFIGURED(MICROKIT_CONFIG_USE_TIMEOUT)
   MicrokitTimeoutInterface timeout;
#endif

} MicrokitInterface;

extern MicrokitInterface Microkit;

#endif
