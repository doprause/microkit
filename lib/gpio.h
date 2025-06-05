/**
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
 * @file gpio.h
 * @author Dominik Prause
 * @version 0.1
 * @date 2025-06-04
 * @copyright Copyright (c) 2025 - All rights reserved.
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
 */
#ifndef MICROKIT_GPIO_H
#define MICROKIT_GPIO_H

#include "libs/microkit/lib/core.h"
#include "libs/microkit/lib/types.h"
#include "microkit/config/gpio.h"

#if MICROKIT_IS_CONFIGURED(MICROKIT_CONFIG_USE_GPIO)

/**
 * @brief UART baudrate settings.
 */
typedef enum {
   MKIT_UART_BAUDRATE_9600,
   MKIT_UART_BAUDRATE_19200,
   MKIT_UART_BAUDRATE_38400,
   MKIT_UART_BAUDRATE_115200,
   MKIT_UART_BAUDRATE_230400,
   MKIT_UART_BAUDRATE_460800,
   MKIT_UART_BAUDRATE_921600
} MicrokitUartBaudRate;

/**
 * @brief UART data bits settings.
 */
typedef enum {
   MKIT_UART_DATABITS_8,
} MicrokitUartDataBits;

/**
 * @brief UART stop bits settings.
 */
typedef enum {
   MKIT_UART_STOPBITS_1,
   MKIT_UART_STOPBITS_2,
} MicrokitUartStopBits;

/**
 * @brief UART parity settings.
 */
typedef enum {
   MKIT_UART_PARITY_NONE,
   MKIT_UART_PARITY_EVEN,
   MKIT_UART_PARITY_ODD
} MicrokitUartParity;

/**
 * @brief UART configuration.
 */
typedef struct {
   const char* format;
   const MicrokitUartBaudRate baudrate;
   const MicrokitUartDataBits databits;
   const MicrokitUartStopBits stopbits;
   const MicrokitUartParity parity;
} MicrokitUartConfig;

/**
 * @brief UART device pointer.
 */
typedef struct UartDeviceObject* MicrokitUartDevice;

/**
 * @brief The UART driver interface.
 */
typedef struct {
   /**
    * @brief Initializes the enabled UART devices.
    * UART devices can be enabled in the MCU configuration file.
    */
   void (*init)(void);

   /**
    * @brief Starts and initializes the given UART device with the given
    * configuration.
    * @param device The device.
    * @param config The config.
    */
   void (*start)(const MicrokitUartDevice device, MicrokitUartConfig config);

   /**
    * @brief Stops and de-initializes the given UART device.
    * @param device The device.
    */
   void (*stop)(const MicrokitUartDevice device);

   /**
    * @brief Sends the given data over the UART device.
    * @param device The device.
    * @param data The data to be sent.
    * @param length The length of the data to be sent.
    * @return The number of bytes sent.
    */
   Int (*send)(const MicrokitUartDevice device, UInt8* data, Size length);

   /**
    * @brief Receives up to maxLength bytes from the UART device.
    * @param device The device.
    * @param data The received data.
    * @param maxLength The maximum number of bytes to receive.
    * @return The number of bytes actually received.
    */
   Int (*receive)(const MicrokitUartDevice device, UInt8* data, Size maxLength);

} MicrokitDriverUartInterface;

#endif // MICROKIT_CONFIG_USE_UART
#endif // MICROKIT_GPIO_H
