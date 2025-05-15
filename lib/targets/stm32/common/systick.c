/** - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
 * @file systick.c
 * @author D. Prause
 * @brief Systick platform API.
 * @version 0.1
 * @date 2025-05-13
 * @copyright Copyright (c) 2025
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#include "libs/microkit/lib/platform/systick.h"

#include "libs/microkit/lib/assert.h"
#include "libs/microkit/lib/debug.h"

#include <stdlib.h>
#include <stm32h533xx.h>
#include <stm32h5xx_hal.h>

#define MAX_SYSTICK_HANDLERS 4

static const UInt32 SYSTICK_INTERVAL_USEC = 10000;

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
struct SystickObject {
   bool isEnabled;
   SystickHandler handlers[MAX_SYSTICK_HANDLERS];
   void* handlerTargets[MAX_SYSTICK_HANDLERS];
   UInt16 tickValue;
};

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
static struct SystickObject instance;

static void init(void) {
   instance.isEnabled = false;
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
static void start(void) {

   instance.isEnabled = true;

   // STM32 HAL doesn't provide an API to enable/disable the systick timer.
   // Enable/disable is implemented by either invoking or not invoking the
   // registered handlers.
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
static void stop(void) {

   instance.isEnabled = false;

   // STM32 HAL doesn't provide an API to enable/disable the systick timer.
   // Enable/disable is implemented by either invoking or not invoking the
   // registered handlers.
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
static UInt16 interval(void) {

   return (UInt16)(SYSTICK_INTERVAL_USEC / 1000);
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
static UInt16 tickValue(void) {

   return instance.tickValue;
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
static void addTickHandler(const SystickHandler handler, void* target) {

   ASSERT_NOT_NULL_POINTER(handler);
   ASSERT_NOT_NULL_POINTER(target);

   for (int i = 0; i < MAX_SYSTICK_HANDLERS; i++) {
      if (instance.handlers[i] == NULL) {
         instance.handlers[i] = handler;
         instance.handlerTargets[i] = target;
         break;
      } else {
         // If the assertion fails here, the array is full.
         ASSERT(i < (MAX_SYSTICK_HANDLERS - 1));
      }
   }
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
static void removeTickHandler(const SystickHandler handler) {

   ASSERT_NOT_NULL_POINTER(handler);

   for (int i = 0; i < MAX_SYSTICK_HANDLERS; i++) {
      if (instance.handlers[i] == handler) {
         instance.handlers[i] = NULL;
         instance.handlerTargets[i] = NULL;
         break;
      }
   }
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
const SystickInterface Systick = {
    .init = init,
    .stop = stop,
    .start = start,
    .interval = interval,
    .tickValue = tickValue,
    .addTickHandler = addTickHandler,
    .removeTickHandler = removeTickHandler};

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
void HAL_SYSTICK_Callback(void) {

   // This function is called on every system timer tick

   static UInt32 periodCounter = 0;

   int SYSTICK_TIMER_PERIOD_USEC = 1000 * HAL_GetTickFreq(); // HAL_GetTickFreq() actually returns the period in msec

   // If SYSTICK_INTERVAL_USEC is greater than the STM32 SYSTICK timer period,
   // the period counter is incremented until the SYSTICK_INTERVAL_USEC has passed.
   if ((periodCounter * SYSTICK_TIMER_PERIOD_USEC) < SYSTICK_INTERVAL_USEC) {
      periodCounter++;
      return;
   }

   periodCounter = 0;

   instance.tickValue = instance.tickValue < UINT16_MAX ? (instance.tickValue + 1) : 0;

   // Call all registered tick handlers
   if (instance.isEnabled) {
      for (int i = 0; i < MAX_SYSTICK_HANDLERS; i++) {
         if (instance.handlers[i] != NULL) {
            instance.handlers[i](
                instance.tickValue,
                instance.handlerTargets[i]);
         }
      }
   }
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
// EOF
