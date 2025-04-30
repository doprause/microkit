#ifndef MICROKIT_H
#define MICROKIT_H

#include "libs/microkit/lib/console.h"
#include "libs/microkit/lib/core.h"
#include "libs/microkit/lib/time.h"


typedef struct {
   void (*init)(void);
   void (*start)(void);
   void (*stop)(void);

#if MICROKIT_IS_CONFIGURED(MICROKIT_CONFIG_USE_CONSOLE)
   MicrokitConsoleInterface console;
#endif

#if MICROKIT_IS_CONFIGURED(MICROKIT_CONFIG_USE_TIME)
   MicrokitTimeInterface time;
#endif
} MicrokitInterface;

extern MicrokitInterface Microkit;

#endif
