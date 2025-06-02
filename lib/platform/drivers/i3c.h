#ifndef MICROKIT_DRIVER_I3C_H
#define MICROKIT_DRIVER_I3C_H

#include "libs/microkit/lib/core.h"
#include "libs/microkit/lib/i3c.h"
#include "microkit/config/i3c.h"

#if MICROKIT_IS_CONFIGURED(MICROKIT_CONFIG_USE_I3C)

void microkit_i3c_init(void);
void microkit_i3c_start(const MicrokitI3cDevice device, I3cConfig config);
void microkit_i3c_stop(const MicrokitI3cDevice device);
void microkit_i3c_process(const MicrokitI3cDevice device);

// StatusOrNumber microkit_i2c_receive(
//     const MicrokitI2cDevice device, UInt8 deviceAddress,
//     UInt8* data, Size dataSize, Bool async);

StatusOrNumber microkit_i3c_transmit(
    const MicrokitI3cDevice device, const UInt8 deviceAddress,
    UInt8* data, Size dataSize, Bool async);

// StatusOrNumber microkit_i2c_memory_read(
//     const MicrokitI2cDevice device, const UInt8 deviceAddress,
//     const UInt16 memoryAddress, const UInt16 memoryAddressSize,
//     UInt8* data, const Size dataSize, const Bool async);

StatusOrNumber microkit_i3c_memory_write(
    const MicrokitI3cDevice device, const UInt8 deviceAddress,
    const UInt16 memoryAddress, const UInt16 memoryAddressSize,
    const UInt8* data, const Size dataSize, const Bool async);

#endif // MICROKIT_CONFIG_USE_I3C
#endif // MICROKIT_DRIVER_I3C_H
