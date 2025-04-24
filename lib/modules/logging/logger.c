
#include "logger.h"
#include "microkit/lib/debug.h"
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

typedef struct {
   int seconds;          /* seconds,  range 0 to 59          */
   int minutes;          /* minutes, range 0 to 59           */
   int hours;            /* hours, range 0 to 23             */
   int monthday;         /* day of the month, range 1 to 31  */
   int month;            /* month, range 0 to 11             */
   int year;             /* The number of years since 1900   */
   int weekday;          /* day of the week, range 0 to 6    */
   int yearday;          /* day in the year, range 0 to 365  */
   int isDaylightSaving; /* daylight saving time             */
} Time;

typedef struct {
   va_list args;
   const char* format;
   const char* file;
   int line;
   int level;
   Time time;
} LogEvent;

typedef struct {
   const char* format;
   LogLevel level;
} Private;

Private private = {
    .format = "",
    .level = MKIT_LOG_LEVEL_TRACE,
};

// static char* private_format;
static const char*
    private_level_strings[] = {"TRACE", "DEBUG", "INFO", "WARN", "ERROR", "FATAL"};

#ifdef MICROKIT_CONFIG_LOGGER_USE_COLORS
static const char* private_level_colors[] = {
    "\033[94m", "\033[36m", "\033[32m", "\033[33m", "\033[31m", "\033[35m"};
#endif

static void private_log(const LogLevel level, const char* file, int line, const char* message, va_list args) {

   char formatted_time[16];
   // time_t t = time(NULL);
   // formatted_time[strftime(formatted_time, sizeof(formatted_time), "%H:%M:%S", localtime(&t))] = '\0';
   // formatted_time[strftime(formatted_time, sizeof(formatted_time), "%H:%M:%S", time(NULL))] = '\0';
   strncpy(formatted_time, "12:34:56", sizeof(formatted_time));

   char formatted_message[64];
   formatted_message[vsnprintf(formatted_message, sizeof(formatted_message), message, args)] = '\0';

#if MICROKIT_CONFIG_LOGGER_USE_SOURCE_LINES == TRUE
#if MICROKIT_CONFIG_LOGGER_USE_COLORS
   const char* format = "%s %s%-5s\033[0m \033[90m%s:%d\033[0m %s\n";
   debug_print(format, formatted_time, private_level_colors[level], private_level_strings[level], file, line, formatted_message);
#else
   const char* format = "%s %-5s %s:%d %s\n";
   debug_print(format, formatted_time, private_level_strings[level], file, line, formatted_message);
#endif
#else
#if MICROKIT_CONFIG_LOGGER_USE_COLORS == TRUE
   const char* format = "%s %s%-5s\033[0m %s\n";
   debug_print(format, formatted_time, private_level_colors[level], private_level_strings[level], formatted_message);
#else
   const char* format = "%s %-5s %s\n";
   debug_print(format, formatted_time, private_level_strings[level], formatted_message);
#endif
#endif
}

static void init(void) {}

#define FILE __FILE__
#define LINE __LINE__

static void write(const LogLevel level, const char* message, ...) {

   if (level < private.level) {
      return;
   }

   va_list args;
   va_start(args, message);
   private_log(level, FILE, LINE, message, args);
   va_end(args);
}

MicroKitLoggerInterface Logger = {
    .init = init,
    .log = write,
};
