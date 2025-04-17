
#include "contracts/platform/drivers/uart.h"
#include "assert.h"
#include "config/mcu.h"
#include "config/os.h"
#include "contracts/platform/sysio.h"

#include "stm32h5xx_hal.h"

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
   👉 Types
   - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
typedef enum {
   MKIT_UART_STATE_CREATED,
   MKIT_UART_STATE_STOPPED,
   MKIT_UART_STATE_STARTED
} UartState;

/**
 * @brief UART device.
 */
struct UartDeviceObject {
   UartState state;
   UART_HandleTypeDef mcu;
};

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
   👉 Configuration
   - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
#if CONFIG_MCU_USE_UART1
struct UartDeviceObject DEVICE_UART1 = {.state = MKIT_UART_STATE_CREATED,
                                        .mcu = {0}};
#endif

#if CONFIG_MCU_USE_UART2
struct UartDeviceObject DEVICE_UART2 = {.state = MKIT_UART_STATE_CREATED,
                                        .mcu = {0}};
#endif

#if defined(CONFIG_OS_CONSOLE_ENABLE) && defined(CONFIG_OS_CONSOLE_USE_UART1)
#if defined(CONFIG_MCU_USE_UART1)
UartDevice DEVICE_CONSOLE = &DEVICE_UART1;
#else
#error                                                                         \
    "To use USART1 for the console, configure CONFIG_MCU_USE_UART1 in config/mcu.h"
#endif
#endif

#if defined(CONFIG_OS_CONSOLE_ENABLE) && defined(CONFIG_OS_CONSOLE_USE_UART2)
#if defined(CONFIG_MCU_USE_UART2)
const UartDevice DEVICE_CONSOLE = &DEVICE_UART2;
#else
#error                                                                         \
    "To use USART2 for the console, configure CONFIG_MCU_USE_UART2 in config/mcu.h"
#endif
#endif

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
   👉 Forward declarations
   - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
static uint uart_convert_baudrate_for_mcu(UartBaudRate baudrate);
static uint uart_convert_databits_for_mcu(UartDataBits databits);
static uint uart_convert_stopbits_for_mcu(UartStopBits stopbits);
static uint uart_convert_parity_for_mcu(UartParity parity);

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
   👉 UART interface implementation
   - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
void uart_init() {

#if CONFIG_MCU_USE_UART1
   DEVICE_UART1.mcu.Instance = USART1;
   DEVICE_UART1.mcu.Init.BaudRate = 115200;
   DEVICE_UART1.mcu.Init.WordLength = UART_WORDLENGTH_8B;
   DEVICE_UART1.mcu.Init.StopBits = UART_STOPBITS_1;
   DEVICE_UART1.mcu.Init.Parity = UART_PARITY_NONE;
   DEVICE_UART1.mcu.Init.Mode = UART_MODE_TX_RX;
   DEVICE_UART1.mcu.Init.HwFlowCtl = UART_HWCONTROL_NONE;
   DEVICE_UART1.mcu.Init.OverSampling = UART_OVERSAMPLING_16;
   DEVICE_UART1.mcu.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
   DEVICE_UART1.mcu.Init.ClockPrescaler = UART_PRESCALER_DIV1;
   DEVICE_UART1.mcu.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;

   DEVICE_UART1.state = MKIT_UART_STATE_STOPPED;
#endif

#if CONFIG_MCU_USE_UART2
   DEVICE_UART2.mcu.Instance = USART2;
   DEVICE_UART2.mcu.Init.BaudRate = 115200;
   DEVICE_UART2.mcu.Init.WordLength = UART_WORDLENGTH_8B;
   DEVICE_UART2.mcu.Init.StopBits = UART_STOPBITS_1;
   DEVICE_UART2.mcu.Init.Parity = UART_PARITY_NONE;
   DEVICE_UART2.mcu.Init.Mode = UART_MODE_TX_RX;
   DEVICE_UART2.mcu.Init.HwFlowCtl = UART_HWCONTROL_NONE;
   DEVICE_UART2.mcu.Init.OverSampling = UART_OVERSAMPLING_16;
   DEVICE_UART2.mcu.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
   DEVICE_UART2.mcu.Init.ClockPrescaler = UART_PRESCALER_DIV1;
   DEVICE_UART2.mcu.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;

   DEVICE_UART2.state = MKIT_UART_STATE_STOPPED;
#endif
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
void uart_start(const UartDevice device, UartConfig config) {

   ASSERT_NOT_NULL_POINTER(device);

   device->mcu.Init.BaudRate = uart_convert_baudrate_for_mcu(config.baudrate);
   device->mcu.Init.WordLength = uart_convert_databits_for_mcu(config.databits);
   device->mcu.Init.StopBits = uart_convert_stopbits_for_mcu(config.stopbits);
   device->mcu.Init.Parity = uart_convert_parity_for_mcu(config.parity);

   bool ok = false;

   ok = HAL_UART_Init(&device->mcu) == HAL_OK;
   ASSERT(ok);

   ok = HAL_UARTEx_SetTxFifoThreshold(&device->mcu,
                                      UART_TXFIFO_THRESHOLD_1_8) == HAL_OK;
   ASSERT(ok);

   ok = HAL_UARTEx_SetRxFifoThreshold(&device->mcu,
                                      UART_RXFIFO_THRESHOLD_1_8) == HAL_OK;
   ASSERT(ok);

   ok = HAL_UARTEx_DisableFifoMode(&device->mcu) == HAL_OK;
   ASSERT(ok);

   device->state = MKIT_UART_STATE_STARTED;
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
void uart_stop(const UartDevice device) {

   ASSERT_NOT_NULL_POINTER(device);

   HAL_UART_DeInit(&device->mcu);

   device->state = MKIT_UART_STATE_STOPPED;
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
int uart_receive(const UartDevice device, unsigned char* data, size maxLength) {

   ASSERT_NOT_NULL_POINTER(device);
   ASSERT(device->state == MKIT_UART_STATE_STARTED);

   return HAL_UART_Receive(&device->mcu, data, maxLength, 100) == HAL_OK ? 0
                                                                         : -1;
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
int uart_send(const UartDevice device, unsigned char* data, size length) {

   ASSERT_NOT_NULL_POINTER(device);
   ASSERT(device->state == MKIT_UART_STATE_STARTED);

   return HAL_UART_Transmit(&device->mcu, data, length, 0) == HAL_OK ? 0 : -1;
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
   👉 SYSIO interface implementation
   - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
int MKIT_sysio_get(void) {

   ASSERT(DEVICE_CONSOLE->state == MKIT_UART_STATE_STARTED);

   char c;
   while (HAL_UART_Receive(&DEVICE_CONSOLE->mcu, (unsigned char*)&c, 1, 0) !=
          HAL_OK)
      ;

   return (int)c;
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
void MKIT_sysio_put(int c) {

   ASSERT(DEVICE_CONSOLE->state == MKIT_UART_STATE_STARTED);

   HAL_UART_Transmit(&DEVICE_CONSOLE->mcu, (unsigned char*)&c, 1, 100);
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
   👉 Private functions
   - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
static uint uart_convert_baudrate_for_mcu(UartBaudRate baudrate) {

   switch (baudrate) {
   case MKIT_UART_BAUDRATE_9600:
      return 9600;
   case MKIT_UART_BAUDRATE_19200:
      return 19200;
   case MKIT_UART_BAUDRATE_38400:
      return 38400;
   case MKIT_UART_BAUDRATE_115200:
      return 115200;
   case MKIT_UART_BAUDRATE_230400:
      return 230400;
   case MKIT_UART_BAUDRATE_460800:
      return 460800;
   case MKIT_UART_BAUDRATE_921600:
      return 921600;
   default:
      return 115200;
   }
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
static uint uart_convert_databits_for_mcu(UartDataBits databits) {

   switch (databits) {
   case MKIT_UART_DATABITS_8:
      return UART_WORDLENGTH_8B;
   default:
      return UART_WORDLENGTH_8B;
   }
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
static uint uart_convert_stopbits_for_mcu(UartStopBits stopbits) {

   switch (stopbits) {
   case MKIT_UART_STOPBITS_1:
      return UART_STOPBITS_1;
   case MKIT_UART_STOPBITS_2:
      return UART_STOPBITS_2;
   default:
      return UART_STOPBITS_1;
   }
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
static uint uart_convert_parity_for_mcu(UartParity parity) {

   switch (parity) {
   case MKIT_UART_PARITY_NONE:
      return UART_PARITY_NONE;
   case MKIT_UART_PARITY_EVEN:
      return UART_PARITY_EVEN;
   case MKIT_UART_PARITY_ODD:
      return UART_PARITY_ODD;
   default:
      return UART_PARITY_NONE;
   }
}
