#ifndef MICROKIT_H
#define MICROKIT_H

#include "microkit/lib/time.h"

typedef struct {
   void (*init)(void);
   void (*start)(void);
   void (*stop)(void);

#if defined(MICROKIT_USE_TIME)
   MicrokitTimeInterface time;
#endif
} MicrokitInterface;

extern MicrokitInterface Microkit;

#endif
