#ifndef MKIT_UART_H
#define MKIT_UART_H

#include "types.h"

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

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
} UartBaudRate;

/**
 * @brief UART data bits settings.
 */
typedef enum { MKIT_UART_DATABITS_8 } UartDataBits;

/**
 * @brief UART stop bits settings.
 */
typedef enum { MKIT_UART_STOPBITS_1, MKIT_UART_STOPBITS_2 } UartStopBits;

/**
 * @brief UART parity settings.
 */
typedef enum {
   MKIT_UART_PARITY_NONE,
   MKIT_UART_PARITY_EVEN,
   MKIT_UART_PARITY_ODD
} UartParity;

/**
 * @brief UART configuration.
 */
typedef struct {
   const char* format;
   const UartBaudRate baudrate;
   const UartDataBits databits;
   const UartStopBits stopbits;
   const UartParity parity;
} UartConfig;

/**
 * @brief UART device pointer.
 */
typedef struct UartDeviceObject* UartDevice;

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

/**
 * @brief Initializes the enabled UART devices.
 * UART devices can be enabled in the MCU configuration file.
 */
void uart_init(void);

/**
 * @brief Starts and initializes the given UART device with the given
 * configuration.
 * @param device The device.
 * @param config The config.
 */
void uart_start(const UartDevice device, UartConfig config);

/**
 * @brief Stops and de-initializes the given UART device.
 * @param device The device.
 */
void uart_stop(const UartDevice device);

/**
 * @brief Sends the given data over the UART device.
 * @param device The device.
 * @param data The data to be sent.
 * @param length The length of the data to be sent.
 * @return The number of bytes sent.
 */
int uart_send(const UartDevice device, uint8* data, size length);

/**
 * @brief Receives up to maxLength bytes from the UART device.
 * @param device The device.
 * @param data The received data.
 * @param maxLength The maximum number of bytes to receive.
 * @return The number of bytes actually received.
 */
int uart_receive(const UartDevice device, uint8* data, size maxLength);

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
#endif
