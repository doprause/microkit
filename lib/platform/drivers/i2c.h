#ifndef MICROKIT_DRIVER_I2C_H
#define MICROKIT_DRIVER_I2C_H

#include "libs/microkit/lib/core.h"
#include "libs/microkit/lib/i2c.h"
#include "microkit/config/i2c.h"

#if MICROKIT_IS_CONFIGURED(MICROKIT_CONFIG_USE_I2C)

void microkit_i2c_init(void);
void microkit_i2c_start(const I2cDevice device, I2cConfig config);
void microkit_i2c_stop(const I2cDevice device);

StatusOrNumber microkit_i2c_receive(
    const I2cDevice device, UInt8 deviceAddress,
    UInt8* data, Size dataSize, Bool async);

StatusOrNumber microkit_i2c_transmit(
    const I2cDevice device, const UInt8 deviceAddress,
    UInt8* data, Size dataSize, Bool async);

StatusOrNumber microkit_i2c_memory_read(
    const I2cDevice device, const UInt8 deviceAddress,
    const UInt16 memoryAddress, const UInt16 memoryAddressSize,
    UInt8* data, const Size dataSize, const Bool async);

StatusOrNumber microkit_i2c_memory_write(
    const I2cDevice device, const UInt8 deviceAddress,
    const UInt16 memoryAddress, const UInt16 memoryAddressSize,
    const UInt8* data, const Size dataSize, const Bool async);

#endif // MICROKIT_CONFIG_USE_I2C
#endif // MICROKIT_DRIVER_I2C_H
