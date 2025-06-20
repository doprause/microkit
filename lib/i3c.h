/**
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
 * @file i3c.h
 * @author Dominik Prause
 * @version 0.1
 * @date 2025-05-15
 * @copyright Copyright (c) 2025 - All rights reserved.
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
 */
#pragma once

#if MICROKIT_IS_CONFIGURED(MICROKIT_CONFIG_USE_I3C)

#include "microkit/config/i3c.h"
#include "microkit/config/microkit.h"

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
   👉 I3C type definitions
   - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

// /**
//  * @brief I2C error codes.
//  */
// typedef enum {
//    MKIT_I2C_ERROR,
//    MKIT_I2C_ERROR_BUS_ERROR,
//    MKIT_I2C_ERROR_ACKNOWLEDGE_ERROR,
// } I2cError;

// /**
//  * @brief I2C mode settings.
//  */
// typedef enum { MKIT_I2C_MODE_MASTER,
//                MKIT_I2C_MODE_SLAVE } I2cMode;

// /**
//  * @brief I2C transmission direction.
//  */
// typedef enum { MKIT_I2C_DIRECTION_READ,
//                MKIT_I2C_DIRECTION_WRITE } I2cDirection;

/**
 * I3C device pointer.
 */
typedef struct I3cDeviceObject* MicrokitI3cDevice;

// /**
//  * @brief Handler function for the memory read complete event.
//  */
// typedef void (*I2cMasterMemoryReadCompleteHandler)(const MicrokitI2cDevice device);

// /**
//  * @brief Handler function for the memory write complete event.
//  */
// typedef void (*I2cMasterMemoryWriteCompleteHandler)(const MicrokitI2cDevice device);

// /**
//  * @brief Handler function for the master receive complete event.
//  */
// typedef void (*I2cMasterReceiveCompleteHandler)(const MicrokitI2cDevice device);

// /**
//  * @brief Handler function for the master transmit complete event.
//  */
// typedef void (*I2cMasterTransmitCompleteHandler)(const MicrokitI2cDevice device);

// /**
//  * @brief Handler function to provide send a data byte to the I2C master.
//  */
// typedef void (*I2cSlaveTransmissionStartedHandler)(const MicrokitI2cDevice device,
//                                                    I2cDirection direction,
//                                                    Bool isRepeatedStart);

// /**
//  * @brief Handler function to provide send a data byte to the I2C master.
//  */
// typedef void (*I2cSlaveTransmissionStoppedHandler)(const MicrokitI2cDevice device);

// /**
//  * @brief Handler function to provide send a data byte to the I2C master.
//  */
// typedef void (*I2cSlaveTransmissionErrorHandler)(const MicrokitI2cDevice device,
//                                                  I2cError error);

// /**
//  * @brief Handler function to process a data byte received from the I2C master.
//  */
// typedef void (*I2cSlaveByteReceivedHandler)(const MicrokitI2cDevice device,
//                                             UInt8* byte);

// /**
//  * @brief Handler function to provide send a data byte to the I2C master.
//  */
// typedef void (*I2cSlaveByteRequestedHandler)(const MicrokitI2cDevice device,
//                                              UInt8* byte);

/**
 * @brief Each I3C target has a descriptor that describes the target characteristics.
 */
typedef struct {
   char* targetName;
   UInt32 targetId;
   UInt64 targetBcrDcrPid;
   UInt8 staticAddress;
   UInt8 dynamicAddress;
} I3cTargetDescriptor;

/**
 * @brief I3C configuration.
 */
typedef struct {
   I3cTargetDescriptor* targetDescriptors;
   Size targetDescriptorsCount;
   //    const I2cMode mode;
   //    const UInt32 address;
   //    I2cMasterMemoryReadCompleteHandler memoryReadCompleteHandler;
   //    I2cMasterMemoryWriteCompleteHandler memoryWriteCompleteHandler;
   //    I2cMasterReceiveCompleteHandler masterReceiveCompleteHandler;
   //    I2cMasterTransmitCompleteHandler masterTransmitCompleteHandler;
   //    I2cSlaveTransmissionStartedHandler slaveTransmissionStartedHandler;
   //    I2cSlaveTransmissionStoppedHandler slaveTransmissionStoppedHandler;
   //    I2cSlaveTransmissionErrorHandler slaveTransmissionErrorHandler;
   //    I2cSlaveByteReceivedHandler slaveByteReceivedHandler;
   //    I2cSlaveByteRequestedHandler slaveByteRequestedHandler;
} I3cConfig;

// /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

typedef struct {

   //    /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
   //    👉 I2C lifecycle functions
   //    - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

   /**
    * @brief Initializes the enabled I3C devices.
    * I3C devices can be enabled in the MCU configuration file.
    */
   void (*init)(void);

   /**
    * @brief Starts and initializes the given I3C device with the given
    * configuration.
    * @param device The device.
    * @param config The config.
    */
   void (*start)(const MicrokitI3cDevice device, I3cConfig config);

   /**
    * @brief Stops and de-initializes the given I3C device.
    * @param device The device.
    */
   void (*stop)(const MicrokitI3cDevice device);

   void (*process)(const MicrokitI3cDevice device);

   /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      👉 I3C Controller mode functions
      - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

   //    /**
   //     * @brief Reads up to #dataSize bytes from the slave device.
   //     * @param device The device.
   //     * @param deviceAddress The slave device address.
   //     * @param[out] data The buffer to store the received data.
   //     * @param dataSize The length of the data to be received.
   //     * @param async Whether to perform the operation asynchronously.
   //     * @returns A status code (STATUS_OK on success, STATUS_ERROR on error).
   //     */
   //    StatusOrNumber (*receive)(const MicrokitI2cDevice device, UInt8 deviceAddress,
   //                              UInt8* data, Size dataSize, Bool async);

   /**
    * @brief Writes #dataSize bytes to the slave device.
    * @param device The device.
    * @param deviceAddress The slave deviceAddress.
    * @param data The data to transmit.
    * @param dataSize The length of the data to be transmitted.
    * @param async Whether to perform the operation asynchronously.
    * @returns A status code (STATUS_OK on success, STATUS_ERROR on error).
    */
   StatusOrNumber (*transmit)(const MicrokitI3cDevice device, UInt8 deviceAddress,
                              UInt8* data, Size dataSize, Bool async);

   //    /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
   //       👉 I2C master mode memory read/write functions
   //       - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

   //    /**
   //     * @brief Reads up to #dataSize bytes from the slave device.
   //     * @param device The device.
   //     * @param deviceAddress The slave device address.
   //     * @param memoryAddress The memory address within the slave device to read from.
   //     * @param[out] data The buffer to store the received data.
   //     * @param dataSize The maximum size of the data to be received.
   //     * @returns A status code (STATUS_OK on success, STATUS_ERROR on error)
   //     * or the number of bytes actually read.
   //     */
   //    StatusOrNumber (*memoryRead)(const MicrokitI2cDevice device, const UInt8 deviceAddress,
   //                                 const UInt16 memoryAddress, const UInt16 memoryAddressSize,
   //                                 UInt8* data, const Size dataSize, const Bool async);

   /**
    * @brief Writes up to #dataSize bytes to the target device.
    * @param device The device.
    * @param deviceAddress The target address.
    * @param memoryAddress The memory address within the target device to write to.
    * @param[out] data The buffer to write the data from.
    * @param length The length of the data to be written.
    * @returns A status code (STATUS_OK on success, STATUS_ERROR on error)
    * or the number of bytes actually written.
    */
   StatusOrNumber (*memoryWrite)(const MicrokitI3cDevice device, const UInt8 deviceAddress,
                                 const UInt16 memoryAddress, const UInt16 memoryAddressSize,
                                 const UInt8* data, const Size dataSize, const Bool async);

} MicrokitDriverI3cInterface;

#endif // MICROKIT_CONFIG_USE_I2C
