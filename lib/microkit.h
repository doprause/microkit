#pragma once

#include "libs/microkit/lib/assert.h"
#include "libs/microkit/lib/console.h"
#include "libs/microkit/lib/core.h"
#include "libs/microkit/lib/debug.h"
#include "libs/microkit/lib/gpio.h"
#include "libs/microkit/lib/i2c.h"
#include "libs/microkit/lib/i3c.h"
#include "libs/microkit/lib/logger.h"
#include "libs/microkit/lib/shell.h"
#include "libs/microkit/lib/time.h"
#include "libs/microkit/lib/timeout.h"
#include "libs/microkit/lib/types.h"
#include "libs/microkit/lib/uart.h"
#include "libs/microkit/lib/utils/termcolor.h"

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
   👉 Types
   - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
typedef void (*InitCallback)(void);

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
   👉 Driver API
   - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
typedef struct {

#if MICROKIT_IS_CONFIGURED(MICROKIT_CONFIG_USE_GPIO)
	MicrokitDriverGpioApi gpio;
#endif

#if MICROKIT_IS_CONFIGURED(MICROKIT_CONFIG_USE_I2C)
   MicrokitDriverI2cInterface i2c;
#endif

#if MICROKIT_IS_CONFIGURED(MICROKIT_CONFIG_USE_I2C)
   MicrokitDriverI3cInterface i3c;
#endif

#if MICROKIT_IS_CONFIGURED(MICROKIT_CONFIG_USE_UART)
   MicrokitDriverUartInterface uart;
#endif

} MicrokitDriverInterface;

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
   👉 Microkit API
   - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
typedef struct {
   void (*init)(InitCallback before, InitCallback after);
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
