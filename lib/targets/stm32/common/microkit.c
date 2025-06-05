/**
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
 * @file microkit.c
 * @author Dominik Prause
 * @version 0.1
 * @date 2025-04-17
 * @copyright Copyright (c) 2025 - All rights reserved.
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
 */
#include "libs/microkit/lib/assert.h"
#include "libs/microkit/lib/core.h"
#include "libs/microkit/lib/microkit.h"
#include "libs/microkit/lib/modules/console/console.h"
#include "libs/microkit/lib/modules/logger/logger.h"
#include "libs/microkit/lib/modules/shell/shell.h"
#include "libs/microkit/lib/platform/drivers/i2c.h"
#include "libs/microkit/lib/platform/drivers/i3c.h"
#include "libs/microkit/lib/platform/drivers/uart.h"
#include "libs/microkit/lib/platform/time.h"

#include "stm32h5xx_hal.h"

// void SystemClock_Config(void);
// static void MX_GPIO_Init(void);
// static void MX_ICACHE_Init(void);

extern void SystemClock_Config(void);
extern void MX_GPIO_Init(void);
extern void MX_ICACHE_Init(void);

static void init(InitCallback before, InitCallback after) {

   before();
   /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
//    HAL_Init();

//    /* Early configuration */
//    SystemClock_Config();
//    MX_GPIO_Init();
//    MX_ICACHE_Init();

/* Initialize all configured peripherals */
#if MICROKIT_IS_CONFIGURED(MICROKIT_CONFIG_USE_I2C)
   Microkit.driver.i2c.init();
#endif

   after();
}

static void start(void) {
   // Nothing to do right now
}

static void stop(void) {
   // Nothing to do right now
}

MicrokitInterface Microkit = {
    .init = init,
    .start = start,
    .stop = stop,

    .driver = {
#if MICROKIT_IS_CONFIGURED(MICROKIT_CONFIG_USE_I2C)
        .i2c = {
            .init = microkit_i2c_init,
            .start = microkit_i2c_start,
            .stop = microkit_i2c_stop,
            .receive = microkit_i2c_receive,
            .transmit = microkit_i2c_transmit,
            .memoryRead = microkit_i2c_memory_read,
            .memoryWrite = microkit_i2c_memory_write,
        },
#endif

#if MICROKIT_IS_CONFIGURED(MICROKIT_CONFIG_USE_I3C)
        .i3c = {
            .init = microkit_i3c_init,
            .start = microkit_i3c_start,
            .stop = microkit_i3c_stop,
            .process = microkit_i3c_process,
            .transmit = microkit_i3c_transmit,
            .memoryWrite = microkit_i3c_memory_write,
        },
#endif

#if MICROKIT_IS_CONFIGURED(MICROKIT_CONFIG_USE_UART)
        .uart = {
            .init = microkit_uart_init,
            .start = microkit_uart_start,
            .stop = microkit_uart_stop,
            .receive = microkit_uart_receive,
            .send = microkit_uart_send,
        },
#endif
    },

#if MICROKIT_IS_CONFIGURED(MICROKIT_CONFIG_USE_CONSOLE)
    .console = {
        .init = microkit_console_init,
        .start = microkit_console_start,
        .stop = microkit_console_stop,
        .read = microkit_console_read,
        .write = microkit_console_write,
        .writeLine = microkit_console_write_line,
    },
#endif

#if MICROKIT_IS_CONFIGURED(MICROKIT_CONFIG_USE_LOGGER)
    .logger = {
        .init = microkit_logger_init,
        .start = microkit_logger_start,
        .stop = microkit_logger_stop,
        .log = microkit_logger_log,
        .getLevel = microkit_logger_get_level,
        .setLevel = microkit_logger_set_level,
    },
#endif

#if MICROKIT_IS_CONFIGURED(MICROKIT_CONFIG_USE_SHELL)
    .shell = {
        .init = microkit_shell_init,
        .start = microkit_shell_start,
        .stop = microkit_shell_stop,
        .process = microkit_shell_process,
        .write = microkit_shell_write,
        .writeError = microkit_shell_write_error,
        .writeLine = microkit_shell_write_line,
        .writeNewLine = microkit_shell_write_new_line,
    },
#endif

#if MICROKIT_IS_CONFIGURED(MICROKIT_CONFIG_USE_TIME)
    .time = {.delay = {
#if MICROKIT_IS_CONFIGURED(MICROKIT_CONFIG_TIME_USE_DELAY_MICROS)
                 .micros = microkit_time_delay_micros,
#endif // MICROKIT_CONFIG_TIME_USE_DELAY_MICROS
                 .millis = microkit_time_delay_millis,
             }},
#endif // MICROKIT_CONFIG_USE_TIME
};

// /**
//  * @brief System Clock Configuration
//  * @retval None
//  */
// void SystemClock_Config(void) {
//    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
//    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

//    /** Configure the main internal regulator output voltage
//     */
//    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);

//    while (!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY)) {
//    }

//    /** Initializes the RCC Oscillators according to the specified parameters
//     * in the RCC_OscInitTypeDef structure.
//     */
//    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
//    RCC_OscInitStruct.HSIState = RCC_HSI_ON;
//    RCC_OscInitStruct.HSIDiv = RCC_HSI_DIV2;
//    RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
//    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;

//    bool ok = HAL_RCC_OscConfig(&RCC_OscInitStruct) == HAL_OK;
//    ASSERT(ok);

//    /** Initializes the CPU, AHB and APB buses clocks
//     */
//    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2 | RCC_CLOCKTYPE_PCLK3;
//    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
//    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
//    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
//    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
//    RCC_ClkInitStruct.APB3CLKDivider = RCC_HCLK_DIV1;

//    ok = HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) == HAL_OK;
//    ASSERT(ok);

//    /** Configure the programming delay
//     */
//    __HAL_FLASH_SET_PROGRAM_DELAY(FLASH_PROGRAMMING_DELAY_0);
// }

// /**
//  * @brief ICACHE Initialization Function
//  * @param None
//  * @retval None
//  */
// static void MX_ICACHE_Init(void) {
//    if (HAL_ICACHE_Enable() != HAL_OK) {
//       // Error_Handler();
//    }
// }

// /**
//  * @brief GPIO Initialization Function
//  * @param None
//  * @retval None
//  */
// static void MX_GPIO_Init(void) {
//    GPIO_InitTypeDef GPIO_InitStruct = {0};
//    /* USER CODE BEGIN MX_GPIO_Init_1 */

//    /* USER CODE END MX_GPIO_Init_1 */

//    /* GPIO Ports Clock Enable */
//    __HAL_RCC_GPIOA_CLK_ENABLE();
//    __HAL_RCC_GPIOC_CLK_ENABLE();
//    __HAL_RCC_GPIOB_CLK_ENABLE();

//    /*Configure GPIO pin : PC4 */
//    GPIO_InitStruct.Pin = GPIO_PIN_4;
//    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
//    GPIO_InitStruct.Pull = GPIO_NOPULL;
//    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
// }
