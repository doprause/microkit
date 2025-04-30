#include "libs/microkit/lib/assert.h"
#include "libs/microkit/lib/core.h"
#include "libs/microkit/lib/debug.h"
#include "libs/microkit/lib/logger.h"
#include "logger.h"
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#if MICROKIT_IS_CONFIGURED(MICROKIT_CONFIG_USE_LOGGER)

#define FILE __FILE__
#define LINE __LINE__

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
   ModuleState moduleState;
   const char* format;
   MicrokitLogLevel level;
} Private;

static Private private = {
    .moduleState = MKIT_MODULE_STATE_UNINITIALIZED,
    .format = "",
    .level = MKIT_LOG_LEVEL_TRACE,
};

// static char* private_format;
static const char*
    private_level_strings[] = {"TRACE", "DEBUG", "INFO", "WARN", "ERROR", "FATAL", "FORCE"};

#ifdef MICROKIT_CONFIG_LOGGER_USE_COLORS
static const char* private_level_colors[] = {
    "\033[94m", "\033[36m", "\033[32m", "\033[33m", "\033[31m", "\033[35m", "\033[0m"};
#endif

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
static void private_log(const MicrokitLogLevel level, const char* file, int line, const char* message, va_list args) {

   char formatted_time[16];
#if MICROKIT_IS_CONFIGURED(MICROKIT_CONFIG_LOGGER_USE_TIMESTAMPS)
   // time_t t = time(NULL);
   // formatted_time[strftime(formatted_time, sizeof(formatted_time), "%H:%M:%S", localtime(&t))] = '\0';
   // formatted_time[strftime(formatted_time, sizeof(formatted_time), "%H:%M:%S", time(NULL))] = '\0';
   strncpy(formatted_time, "--:--:--", sizeof(formatted_time));
#else
   strncpy(formatted_time, "", sizeof(formatted_time));
#endif // MICROKIT_CONFIG_LOGGER_USE_TIMESTAMPS

   char formatted_message[64];
   formatted_message[vsnprintf(formatted_message, sizeof(formatted_message), message, args)] = '\0';

#if MICROKIT_IS_CONFIGURED(MICROKIT_CONFIG_LOGGER_USE_SOURCE_LINES)
#if MICROKIT_IS_CONFIGURED(MICROKIT_CONFIG_LOGGER_USE_COLORS)
   const char* format = "%s %s%-5s\033[0m \033[90m%s:%d\033[0m %s\n";
   debug_print(format, formatted_time, private_level_colors[level], private_level_strings[level], file, line, formatted_message);
#else
   const char* format = "%s %-5s %s:%d %s\n";
   debug_print(format, formatted_time, private_level_strings[level], file, line, formatted_message);
#endif // MICROKIT_IS_CONFIGURED(MICROKIT_CONFIG_LOGGER_USE_COLORS)
#else
#if MICROKIT_IS_CONFIGURED(MICROKIT_CONFIG_LOGGER_USE_COLORS)
   const char* format = "%s %s%-5s\033[0m %s\n";
   debug_print(format, formatted_time, private_level_colors[level], private_level_strings[level], formatted_message);
#else
   const char* format = "%s %-5s %s\n";
   debug_print(format, formatted_time, private_level_strings[level], formatted_message);
#endif // MICROKIT_IS_CONFIGURED(MICROKIT_CONFIG_LOGGER_USE_COLORS)
#endif // MICROKIT_IS_CONFIGURED(MICROKIT_CONFIG_LOGGER_USE_SOURCE_LINES)
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
void microkit_logger_init(void) {

   ASSERT(private.moduleState == MKIT_MODULE_STATE_UNINITIALIZED);

   private.moduleState = MKIT_MODULE_STATE_STOPPED;
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
void microkit_logger_start(void) {

   ASSERT(private.moduleState != MKIT_MODULE_STATE_UNINITIALIZED);

   if (private.moduleState == MKIT_MODULE_STATE_STOPPED) {
      private.moduleState = MKIT_MODULE_STATE_RUNNING;
   }
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
void microkit_logger_stop(void) {

   ASSERT(private.moduleState != MKIT_MODULE_STATE_UNINITIALIZED);

   if (private.moduleState == MKIT_MODULE_STATE_RUNNING) {
      private.moduleState = MKIT_MODULE_STATE_STOPPED;
   }
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
void microkit_logger_log(const MicrokitLogLevel level, const char* message, ...) {

   if (level < private.level) {
      return;
   }

   if (level >= private.level) {
      va_list args;
      va_start(args, message);
      private_log(level, FILE, LINE, message, args);
      va_end(args);
   }
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
MicrokitLogLevel microkit_logger_get_level(void) {
   return private.level;
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
void microkit_logger_set_level(const MicrokitLogLevel level) {

   if (level >= MKIT_LOG_LEVEL_TRACE && level <= MKIT_LOG_LEVEL_FATAL) {
      private.level = level;
   }
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
// LoggerInterface Logger = {
//     .init = init,
//     .log = write,
//     .getLevel = get_level,
//     .setLevel = set_level,
// };

#endif // MICROKIT_IS_CONFIGURED(MICROKIT_CONFIG_USE_LOGGER)
