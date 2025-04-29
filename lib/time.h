#include "config/microkit.h"
#include "microkit/lib/types.h"

#if defined(MICROKIT_USE_TIME)

typedef struct {
   /**
    * @brief Busy wait for the specified number of microseconds.
    * @param micros Delay in microseconds.
    */
   void (*micros)(UInt32 micros);

       /**
        * @brief Busy wait for the specified number of milliseconds.
        * @param millis Delay in milliseconds.
        */
       void (*millis)(UInt32 millis);
} MicrokitTimeDelayInterface;

typedef struct {
   const MicrokitTimeDelayInterface delay;
} MicrokitTimeInterface;

#endif
