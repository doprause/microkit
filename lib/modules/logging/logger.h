/**
 * @brief
 * Inspired by https://github.com/rxi/log.c and https://github.com/ErichStyger/McuLib/blob/master/src/McuLog.h
 */

#ifndef MICROKIT_LOGGER_H
#define MICROKIT_LOGGER_H

#include "microkit/lib/config/logger.h"
#include "microkit/lib/types.h"

typedef enum {
   MKIT_LOG_LEVEL_TRACE,
   MKIT_LOG_LEVEL_DEBUG,
   MKIT_LOG_LEVEL_INFO,
   MKIT_LOG_LEVEL_WARN,
   MKIT_LOG_LEVEL_ERROR,
   MKIT_LOG_LEVEL_FATAL,
} LogLevel;

typedef struct {
   void (*init)(void);
   void (*log)(const LogLevel level, const char* message, ...);
   LogLevel (*getLevel)(void);
   void (*setLevel)(const LogLevel level);
} MicroKitLoggerInterface;

extern MicroKitLoggerInterface Logger;

#endif