/**
 * @brief A logger.
 * Inspired by https://github.com/rxi/log.c and https://github.com/ErichStyger/McuLib/blob/master/src/McuLog.h
 */

#ifndef MICROKIT_LOGGER_H
#define MICROKIT_LOGGER_H

#include "libs/microkit/lib/core.h"
#include "libs/microkit/lib/types.h"
#include "microkit/config/logger.h"

#if MICROKIT_IS_CONFIGURED(MICROKIT_CONFIG_USE_LOGGER)

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
} MicrokitLogLevel;

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
    * @brief Starts the logger.
    */
   void (*start)(void);

   /**
    * @brief Stops the logger.
    */
   void (*stop)(void);

   /**
    * @brief Logs a message with the given level.
    * @param [in] level The log level.
    * @param [in] message The printf formatted message to be logged.
    * @param  [in] ... The arguments for the message.
    */
   void (*log)(const MicrokitLogLevel level, String message, ...);

   /**
    * @brief Gets the current log level.
    * @returns The current log level.
    */
   MicrokitLogLevel (*getLevel)(void);

   /**
    * @brief Sets the log level.
    * @param [in] level The log level to be set.
    */
   void (*setLevel)(const MicrokitLogLevel level);

} MicrokitLoggerInterface;

#endif // MICROKIT_CONFIG_USE_LOGGER

#endif // MICROKIT_LOGGER_H
