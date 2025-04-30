#include "console.h"
#include "libs/microkit/lib/assert.h"
#include "libs/microkit/lib/core.h"
#include "libs/microkit/lib/platform/stdio.h"

#if MICROKIT_IS_CONFIGURED(MICROKIT_CONFIG_USE_CONSOLE)

typedef struct {
   ModuleState moduleState;
} Private;

static Private private = {
    .moduleState = MKIT_MODULE_STATE_UNINITIALIZED,
};

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
void microkit_console_init(void) {

   ASSERT(private.moduleState == MKIT_MODULE_STATE_UNINITIALIZED);

   private.moduleState = MKIT_MODULE_STATE_STOPPED;
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
void microkit_console_start(void) {

   ASSERT(private.moduleState != MKIT_MODULE_STATE_UNINITIALIZED);

   if (private.moduleState == MKIT_MODULE_STATE_STOPPED) {
      private.moduleState = MKIT_MODULE_STATE_RUNNING;
   }
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
void microkit_console_stop(void) {

   ASSERT(private.moduleState != MKIT_MODULE_STATE_UNINITIALIZED);

   if (private.moduleState == MKIT_MODULE_STATE_RUNNING) {
      private.moduleState = MKIT_MODULE_STATE_STOPPED;
   }
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
int microkit_console_read(char* message, int length) {

   ASSERT(private.moduleState == MKIT_MODULE_STATE_RUNNING);

   char* received = message;
   char c;

   *received = '\000';

   /* read until a <LF> is received */
   while ((c = mkit_platform_stdio_get()) != '\r') {
      *received = c;
      mkit_platform_stdio_put(c);
      if ((received - message) < length) {
         received++;
      }

      /* update end of string with NUL */
      *received = '\000';
   }

   return received - message;
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
void microkit_console_write(const char* message) {

   ASSERT(private.moduleState == MKIT_MODULE_STATE_RUNNING);

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
void microkit_console_write_line(const char* message) {

   ASSERT(private.moduleState == MKIT_MODULE_STATE_RUNNING);

   microkit_console_write(message);
   microkit_console_write("\n");
}

#endif // MICROKIT_CONFIG_USE_CONSOLE
