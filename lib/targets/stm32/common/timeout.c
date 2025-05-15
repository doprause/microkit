/** - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
 * @file timeout.h
 * @author D. Prause
 * @brief Software timer module.
 * @version 0.1
 * @date 2025-05-13
 * @copyright Copyright (c) 2025
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#include "libs/microkit/lib/assert.h"
#include "libs/microkit/lib/core.h"
#include "libs/microkit/lib/platform/systick.h"
#include "libs/microkit/lib/platform/timeout.h"
#include "microkit/config/microkit.h"
#include "microkit/config/timeout.h"

typedef struct {
   UInt16 timeout;
   void* target;
   void (*handler)(void*);
} Timeout;

typedef struct {
   ModuleState moduleState;
   Timeout timeouts[MICROKIT_MAX_TIMOUTS];
} Private;

static Private private = {
    .moduleState = MKIT_MODULE_STATE_UNINITIALIZED,
};

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
void private_reset_timeout(Timeout timeout) {
   timeout.timeout = 0;
   timeout.target = NULL;
   timeout.handler = NULL;
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
void microkit_timeout_init(void) {
   private.moduleState = MKIT_MODULE_STATE_STOPPED;
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
void microkit_timeout_start(void) {

   for (int i = 0; i < MICROKIT_MAX_TIMOUTS; i++) {
      private_reset_timeout(private.timeouts[i]);
   }

   private.moduleState = MKIT_MODULE_STATE_RUNNING;
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
void microkit_timeout_stop(void) {
   private.moduleState = MKIT_MODULE_STATE_STOPPED;
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
void microkit_timeout_process(void) {

   if (private.moduleState != MKIT_MODULE_STATE_RUNNING) {
      return;
   }

   // Read current systick and fire elapsed timeouts
   UInt16 currentTicks = Systick.tickValue();

   for (int i = 0; i < MICROKIT_MAX_TIMOUTS; i++) {
      if (private.timeouts[i].timeout >= currentTicks) {
         private.timeouts[i].handler(private.timeouts[i].target);
         private_reset_timeout(private.timeouts[i]);
      }
   }
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
void microkit_timeout_set(UInt16 timeout, void* target, void (*handler)(void*)) {

   if (private.moduleState != MKIT_MODULE_STATE_RUNNING) {
      return;
   }

   ASSERT(target != NULL);
   ASSERT(handler != NULL);

   UInt16 ticksToTimeout = 1 + (timeout / Systick.interval());
   UInt16 currentTicks = Systick.tickValue();

   private.timeouts[timeout].timeout = currentTicks + ticksToTimeout;
   private.timeouts[timeout].target = target;
   private.timeouts[timeout].handler = handler;
}