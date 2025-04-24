
#include "console.h"
#include "microkit/lib/assert.h"
#include "microkit/lib/contracts/platform/stdio.h"

typedef enum {
   MKIT_MODULE_STATE_UNINITIALIZED,
   MKIT_MODULE_STATE_STOPPED,
   MKIT_MODULE_STATE_RUNNING
} ModuleState;

struct Private {
   ModuleState moduleState;
};

static struct Private private = {
    .moduleState = MKIT_MODULE_STATE_UNINITIALIZED,
};

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
static void init(void) {

   ASSERT(private.moduleState == MKIT_MODULE_STATE_UNINITIALIZED);

   private.moduleState = MKIT_MODULE_STATE_STOPPED;
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
static void start(void) {

   ASSERT(private.moduleState != MKIT_MODULE_STATE_UNINITIALIZED);

   if (private.moduleState == MKIT_MODULE_STATE_STOPPED) {
      private.moduleState = MKIT_MODULE_STATE_RUNNING;
   }
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
static void stop(void) {

   ASSERT(private.moduleState != MKIT_MODULE_STATE_UNINITIALIZED);

   if (private.moduleState == MKIT_MODULE_STATE_RUNNING) {
      private.moduleState = MKIT_MODULE_STATE_STOPPED;
   }
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
static int read(char* message, int length) {

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
static void write(const char* message) {

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
static void write_line(const char* message) {

   ASSERT(private.moduleState == MKIT_MODULE_STATE_RUNNING);

   write(message);
   write("\n");
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
ConsoleInterface Console = {
    .init = init,
    .start = start,
    .stop = stop,
    .read = read,
    .write = write,
    .writeLine = write_line,
};
