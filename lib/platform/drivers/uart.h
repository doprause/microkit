/**
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
 * @file uart.h
 * @author Dominik Prause
 * @version 0.1
 * @date 2025-04-17
 * @copyright Copyright (c) 2025 - All rights reserved.
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
 */
#ifndef MICROKIT_DRIVER_UART_H
#define MICROKIT_DRIVER_UART_H

#include "libs/microkit/lib/core.h"
#include "libs/microkit/lib/uart.h"
#include "microkit/config/uart.h"

#if MICROKIT_IS_CONFIGURED(MICROKIT_CONFIG_USE_UART)

void microkit_uart_init(void);
void microkit_uart_start(const MicrokitUartDevice device, MicrokitUartConfig config);
void microkit_uart_stop(const MicrokitUartDevice device);
int microkit_uart_receive(const MicrokitUartDevice device, UInt8* data, const Size size);
int microkit_uart_send(const MicrokitUartDevice device, UInt8* data, const Size size);

#endif // MICROKIT_CONFIG_USE_UART
#endif // MICROKIT_DRIVER_UART_H
