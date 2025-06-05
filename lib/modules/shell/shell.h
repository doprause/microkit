/**
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
 * @file shell.h
 * @author Dominik Prause
 * @version 0.1
 * @date 2025-04-17
 * @copyright Copyright (c) 2025 - All rights reserved.
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
 */
#pragma once

#if MICROKIT_IS_CONFIGURED(MICROKIT_CONFIG_USE_SHELL)

#include "libs/microkit/lib/shell.h"
#include "libs/microkit/lib/uart.h"
#include "microkit/config/shell.h"

void microkit_shell_init(
    const ShellModule instance,
    const MicrokitUartDevice serial,
    const char* prompt,
    const char terminator);
void microkit_shell_stop(const ShellModule instance);
void microkit_shell_start(const ShellModule instance);
void microkit_shell_process(const ShellModule instance);
void microkit_shell_write(const ShellModule instance, const char* format, ...);
void microkit_shell_write_error(const ShellModule instance, const char* format, ...);
void microkit_shell_write_line(const ShellModule instance, const char* format, ...);
void microkit_shell_write_new_line(const ShellModule instance, const char* format, ...);

#endif // MICROKIT_IS_CONFIGURED(MICROKIT_CONFIG_USE_SHELL)
