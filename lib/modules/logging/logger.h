/**
 * @brief A logger.
 * Inspired by https://github.com/rxi/log.c and https://github.com/ErichStyger/McuLib/blob/master/src/McuLog.h
 */

#ifndef MICROKIT_LOGGER_H
#define MICROKIT_LOGGER_H

#include "microkit/lib/config/logger.h"
#include "microkit/lib/types.h"

/**
 * @brief Log levels.
 */
typedef enum {
   MKIT_LOG_LEVEL_TRACE,
   MKIT_LOG_LEVEL_DEBUG,
   MKIT_LOG_LEVEL_INFO,
   MKIT_LOG_LEVEL_WARN,
   MKIT_LOG_LEVEL_ERROR,
   MKIT_LOG_LEVEL_FATAL,
   MKIT_LOG_LEVEL_FORCE
} LogLevel;

/**
 * @brief The logger interface.
 */
typedef struct {
   /**
    * @brief Inititalizes the logger.
    * @note Must be called before any other function.
    */
   void (*init)(void);

   /**
    * @brief Logs a message with the given level.
    * @param [in] level The log level.
    * @param [in] message The printf formatted message to be logged.
    * @param  [in] ... The arguments for the message.
    */
   void (*log)(const LogLevel level, String message, ...);

   /**
    * @brief Gets the current log level.
    * @returns The current log level.
    */
   LogLevel (*getLevel)(void);

   /**
    * @brief Sets the log level.
    * @param [in] level The log level to be set.
    */
   void (*setLevel)(const LogLevel level);

} LoggerInterface;

/**
 * @brief The singleton logger instance.
 */
extern LoggerInterface Logger;

#endif