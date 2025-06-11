/**
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
 * @file gpio.h
 * @author Dominik Prause
 * @version 0.1
 * @date 2025-06-04
 * @copyright Copyright (c) 2025 - All rights reserved.
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
 */
#pragma once

#include "libs/microkit/lib/core.h"
#include "libs/microkit/lib/types.h"
#include "microkit/config/gpio.h"

#if MICROKIT_IS_CONFIGURED(MICROKIT_CONFIG_USE_GPIO)

/**
 * @brief GPIO device pointer.
 */
typedef struct GpioDeviceObject* MicrokitGpioDevice;

/**
 * @brief The GPIO driver API.
 */
typedef struct {
   /**
    * @brief Initializes the enabled GPIO devices.
    * @note GPIO devices can be enabled in the corresponding configuration file.
    */
   void (*init)(void);

   /**
    * @brief Starts the given GPIO device.
    * @param device The device.
    */
   void (*start)(const MicrokitGpioDevice device);

   /**
    * @brief Stops the given GPIO device.
    * @param device The device.
    */
   void (*stop)(const MicrokitGpioDevice device);

   /**
    * @brief Gets the state of the GPIO pin.
    * @param instance The instance.
    * @returns TRUE, if the GPIO pin is HIGH, FALSE otherwise.
    */
   bool (*get)(const MicrokitGpioDevice device);

   /**
    * @brief Sets the state of the GPIO pin.
    * @param instance The instance.
    * @param on The GPIO state to be set. FALSE means LOW, TRUE means HIGH.
    */
   void (*set)(const MicrokitGpioDevice device, bool on);

   /**
    * @brief Toggles the LED.
    * @param instance The instance.
    */
   void (*toggle)(const MicrokitGpioDevice device);

} MicrokitDriverGpioApi;

#endif // MICROKIT_IS_CONFIGURED(MICROKIT_CONFIG_USE_GPIO)
