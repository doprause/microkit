
#include "console.h"
#include "microkit/lib/contracts/platform/stdio.h"

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
int console_read(char* message, int length) {

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
void console_write(char* message) {

   while (*message != '\0') {
      if (*message == '\r') {
         mkit_platform_stdio_put('\n');
      } else {
         mkit_platform_stdio_put(*message);
      }
      message++;
   }
}
