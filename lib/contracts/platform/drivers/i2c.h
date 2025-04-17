#ifndef MICROKIT_I2C_H
#define MICROKIT_I2C_H

#include "microkit/lib/types.h"

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
   👉 I2C type definitions
   - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

/**
 * @brief I2C error codes.
 */
typedef enum {
   MKIT_I2C_ERROR,
   MKIT_I2C_ERROR_BUS_ERROR,
   MKIT_I2C_ERROR_ACKNOWLEDGE_ERROR,
} I2cError;

/**
 * @brief I2C mode settings.
 */
typedef enum { MKIT_I2C_MODE_MASTER, MKIT_I2C_MODE_SLAVE } I2cMode;

/**
 * @brief I2C transmission direction.
 */
typedef enum { MKIT_I2C_DIRECTION_READ, MKIT_I2C_DIRECTION_WRITE } I2cDirection;

/**
 * I2C device pointer.
 */
typedef struct I2cDeviceObject* I2cDevice;

/**
 * @brief Handler function for the memory read complete event.
 */
typedef void (*I2cMasterMemoryReadCompleteHandler)(const I2cDevice device);

/**
 * @brief Handler function for the memory write complete event.
 */
typedef void (*I2cMasterMemoryWriteCompleteHandler)(const I2cDevice device);

/**
 * @brief Handler function for the master receive complete event.
 */
typedef void (*I2cMasterReceiveCompleteHandler)(const I2cDevice device);

/**
 * @brief Handler function for the master transmit complete event.
 */
typedef void (*I2cMasterTransmitCompleteHandler)(const I2cDevice device);

/**
 * @brief Handler function to provide send a data byte to the I2C master.
 */
typedef void (*I2cSlaveTransmissionStartedHandler)(const I2cDevice device,
                                                   I2cDirection direction,
                                                   Bool isRepeatedStart);

/**
 * @brief Handler function to provide send a data byte to the I2C master.
 */
typedef void (*I2cSlaveTransmissionStoppedHandler)(const I2cDevice device);

/**
 * @brief Handler function to provide send a data byte to the I2C master.
 */
typedef void (*I2cSlaveTransmissionErrorHandler)(const I2cDevice device,
                                                 I2cError error);

/**
 * @brief Handler function to process a data byte received from the I2C master.
 */
typedef void (*I2cSlaveByteReceivedHandler)(const I2cDevice device,
                                            UInt8* byte);

/**
 * @brief Handler function to provide send a data byte to the I2C master.
 */
typedef void (*I2cSlaveByteRequestedHandler)(const I2cDevice device,
                                             UInt8* byte);

/**
 * @brief I2C configuration.
 */
typedef struct {
   const I2cMode mode;
   const UInt32 address;
   I2cMasterMemoryReadCompleteHandler memoryReadCompleteHandler;
   I2cMasterMemoryWriteCompleteHandler memoryWriteCompleteHandler;
   I2cMasterReceiveCompleteHandler masterReceiveCompleteHandler;
   I2cMasterTransmitCompleteHandler masterTransmitCompleteHandler;
   I2cSlaveTransmissionStartedHandler slaveTransmissionStartedHandler;
   I2cSlaveTransmissionStoppedHandler slaveTransmissionStoppedHandler;
   I2cSlaveTransmissionErrorHandler slaveTransmissionErrorHandler;
   I2cSlaveByteReceivedHandler slaveByteReceivedHandler;
   I2cSlaveByteRequestedHandler slaveByteRequestedHandler;
} I2cConfig;

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
   👉 I2C lifecycle functions
   - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

/**
 * @brief Initializes the enabled I2C devices.
 * I2C devices can be enabled in the MCU configuration file.
 */
void i2c_init(void);

/**
 * @brief Starts and initializes the given I2C device with the given
 * configuration.
 * @param device The device.
 * @param config The config.
 */
void i2c_start(const I2cDevice device, I2cConfig config);

/**
 * @brief Stops and de-initializes the given I2C device.
 * @param device The device.
 */
void i2c_stop(const I2cDevice device);

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
   👉 I2C master mode functions
   - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

/**
 * @brief Reads up to #dataSize bytes from the slave device.
 * @param device The device.
 * @param deviceAddress The slave device address.
 * @param[out] data The buffer to store the received data.
 * @param dataSize The length of the data to be received.
 * @param async Whether to perform the operation asynchronously.
 * @returns A status code (STATUS_OK on success, STATUS_ERROR on error).
 */
StatusOrNumber i2c_receive(const I2cDevice device, UInt8 deviceAddress,
                           UInt8* data, Size dataSize, Bool async);

/**
 * @brief Writes #dataSize bytes to the slave device.
 * @param device The device.
 * @param deviceAddress The slave deviceAddress.
 * @param data The data to transmit.
 * @param dataSize The length of the data to be transmitted.
 * @param async Whether to perform the operation asynchronously.
 * @returns A status code (STATUS_OK on success, STATUS_ERROR on error).
 */
StatusOrNumber i2c_transmit(const I2cDevice device, UInt8 deviceAddress,
                            UInt8* data, Size dataSize, Bool async);

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
   👉 I2C master mode memory read/write functions
   - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

/**
 * @brief Reads up to #dataSize bytes from the slave device.
 * @param device The device.
 * @param deviceAddress The slave device address.
 * @param memoryAddress The memory address within the slave device to read from.
 * @param[out] data The buffer to store the received data.
 * @param dataSize The maximum size of the data to be received.
 * @returns A status code (STATUS_OK on success, STATUS_ERROR on error)
 * or the number of bytes actually read.
 */
StatusOrNumber i2c_memory_read(const I2cDevice device, const UInt8 deviceAddress,
                               const UInt16 memoryAddress, const UInt16 memoryAddressSize,
                               UInt8* data, const Size dataSize, const Bool async);

/**
 * @brief Writes up to #dataSize bytes to the slave device.
 * @param device The device.
 * @param deviceAddress The slave device address.
 * @param memoryAddress The memory address within the slave device to write to.
 * @param[out] data The buffer to write the data from.
 * @param length The length of the data to be written.
 * @returns A status code (STATUS_OK on success, STATUS_ERROR on error)
 * or the number of bytes actually written.
 */
StatusOrNumber i2c_memory_write(const I2cDevice device, const UInt8 deviceAddress,
                                const UInt16 memoryAddress, const UInt16 memoryAddressSize,
                                const UInt8* data, const Size dataSize, const Bool async);

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
#endif
