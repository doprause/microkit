/**
 * @brief A logger.
 * Inspired by https://github.com/rxi/log.c and https://github.com/ErichStyger/McuLib/blob/master/src/McuLog.h
 */

#pragma once

#if MICROKIT_IS_CONFIGURED(MICROKIT_CONFIG_USE_LOGGER)

#include "libs/microkit/lib/core.h"
#include "libs/microkit/lib/logger.h"
#include "microkit/config/logger.h"

void microkit_logger_init(void);
void microkit_logger_start(void);
void microkit_logger_stop(void);
void microkit_logger_log(const MicrokitLogLevel level, const char* message, ...);
MicrokitLogLevel microkit_logger_get_level(void);
void microkit_logger_set_level(const MicrokitLogLevel level);

#endif //#if MICROKIT_IS_CONFIGURED(MICROKIT_CONFIG_USE_LOGGER)
