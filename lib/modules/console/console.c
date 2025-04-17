
#include "console.h"
#include "microkit/lib/contracts/platform/drivers/uart.h"
#include "microkit/lib/contracts/platform/sysio.h"

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
void console_log(char* message) {
   console_put("[LOG  ] ");
   console_put(message);
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
void console_warn(char* message) {
   console_put("[WARN ] ");
   console_put(message);
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
void console_error(char* message) {
   console_put("[ERROR] ");
   console_put(message);
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
int console_get(char* message, int length) {

   char* received = message;
   char c;

   *received = '\000';

   /* read until a <LF> is received */
   while ((c = sysio_get()) != '\r') {
      *received = c;
      sysio_put(c);
      if ((received - message) < length) {
         received++;
      }

      /* update end of string with NUL */
      *received = '\000';
   }

   return received - message;
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
void console_put(char* message) {

   while (*message != '\0') {
      if (*message == '\r') {
         sysio_put('\n');
      } else {
         sysio_put(*message);
      }
      message++;
   }
}
