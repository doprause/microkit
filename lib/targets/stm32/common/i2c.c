#include "libs/microkit/lib/assert.h"
#include "libs/microkit/lib/core.h"
#include "libs/microkit/lib/debug.h"
#include "libs/microkit/lib/microkit.h"
#include "libs/microkit/lib/platform/drivers/i2c.h"
#include "microkit/config/i2c.h"

#include "stm32h5xx_hal.h"

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
   👉 Types
   - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
typedef struct {
   bool isRepeatedStart;
   bool memoryReadComplete;
   bool memoryWriteComplete;
   bool masterReceiveComplete;
   bool masterTransmitComplete;
} I2cStatus;

typedef struct {
   I2cMasterMemoryReadCompleteHandler memoryReadCompleteHandler;
   I2cMasterMemoryWriteCompleteHandler memoryWriteCompleteHandler;
   I2cMasterReceiveCompleteHandler masterReceiveCompleteHandler;
   I2cMasterTransmitCompleteHandler masterTransmitCompleteHandler;
   I2cSlaveTransmissionStartedHandler slaveTransmissionStartedHandler;
   I2cSlaveTransmissionStoppedHandler slaveTransmissionStoppedHandler;
   I2cSlaveTransmissionErrorHandler slaveTransmissionErrorHandler;
   I2cSlaveByteReceivedHandler slaveByteReceivedHandler;
   I2cSlaveByteRequestedHandler slaveByteRequestedHandler;
} I2cCallbacks;

typedef struct {
   UInt8 rxByte;
   UInt8 txByte;
} I2cSlaveState;

/**
 * @brief I2C device.
 */
struct I2cDeviceObject {
   DriverState state;
   I2cCallbacks callbacks;
   I2cMode mode;
   I2cStatus status;
   I2cSlaveState slave;
   I2C_HandleTypeDef mcu;
};

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
   👉 Configuration
   - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
#if MICROKIT_IS_CONFIGURED(MICROKIT_CONFIG_I2C_USE_I2C1)
struct I2cDeviceObject DEVICE_I2C1_INSTANCE = {
    .state = MKIT_DRIVER_STATE_UNINITIALIZED,
    .mcu = {0},
};
MicrokitI2cDevice DEVICE_I2C1 = &DEVICE_I2C1_INSTANCE;
#endif // MICROKIT_CONFIG_I2C_USE_I2C1

#if MICROKIT_IS_CONFIGURED(MICROKIT_CONFIG_I2C_USE_I2C2)
struct I2cDeviceObject DEVICE_I2C2_INSTANCE = {
    .state = MKIT_DRIVER_STATE_UNINITIALIZED,
    .mcu = {0},
};
MicrokitI2cDevice DEVICE_I2C2 = &DEVICE_I2C2_INSTANCE;
#endif // MICROKIT_CONFIG_I2C_USE_I2C2

#if MICROKIT_IS_CONFIGURED(MICROKIT_CONFIG_I2C_USE_I2C3)
struct I2cDeviceObject DEVICE_I2C3_INSTANCE = {
    .state = MKIT_DRIVER_STATE_UNINITIALIZED,
    .mcu = {0},
};
MicrokitI2cDevice DEVICE_I2C3 = &DEVICE_I2C3_INSTANCE;
#endif // MICROKIT_CONFIG_I2C_USE_I2C3

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
   👉 Private functions
   - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
static inline void rearm_i2c_slave_mode(const MicrokitI2cDevice device) {

   device->status.isRepeatedStart = false;

   HAL_I2C_EnableListen_IT(&device->mcu);
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
static inline MicrokitI2cDevice get_device_from_handle(I2C_HandleTypeDef* handle) {
#if MICROKIT_IS_CONFIGURED(MICROKIT_CONFIG_I2C_USE_I2C1)
   if (handle == &DEVICE_I2C1->mcu) {
      return DEVICE_I2C1;
   }
#endif
#if MICROKIT_IS_CONFIGURED(MICROKIT_CONFIG_I2C_USE_I2C2)
   if (handle == &DEVICE_I2C2->mcu) {
      return DEVICE_I2C2;
   }
#endif
#if MICROKIT_IS_CONFIGURED(MICROKIT_CONFIG_I2C_USE_I2C3)
   if (handle == &DEVICE_I2C3->mcu) {
      return DEVICE_I2C3;
   }
#endif

   ASSERT_NOT_REACHED;

   return 0;
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
   👉 I2C lifecycle functions
   - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
void microkit_i2c_init(void) {

#if MICROKIT_IS_CONFIGURED(MICROKIT_CONFIG_I2C_USE_I2C1)
   DEVICE_I2C1->mcu.Instance = I2C1;
   DEVICE_I2C1->mcu.Init.Timing = 0x00707CBB;
   DEVICE_I2C1->mcu.Init.OwnAddress1 = 0;
   DEVICE_I2C1->mcu.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
   DEVICE_I2C1->mcu.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
   DEVICE_I2C1->mcu.Init.OwnAddress2 = 0;
   DEVICE_I2C1->mcu.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
   DEVICE_I2C1->mcu.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
   DEVICE_I2C1->mcu.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;

   DEVICE_I2C1->state = MKIT_DRIVER_STATE_STOPPED;
#endif

#if MICROKIT_IS_CONFIGURED(MICROKIT_CONFIG_I2C_USE_I2C2)
   DEVICE_I2C2->mcu.Instance = I2C2;
   DEVICE_I2C2->mcu.Init.Timing = 0x00707CBB;
   DEVICE_I2C2->mcu.Init.OwnAddress1 = 0;
   DEVICE_I2C2->mcu.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
   DEVICE_I2C2->mcu.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
   DEVICE_I2C2->mcu.Init.OwnAddress2 = 0;
   DEVICE_I2C2->mcu.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
   DEVICE_I2C2->mcu.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
   DEVICE_I2C2->mcu.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;

   DEVICE_I2C2->state = MKIT_DRIVER_STATE_STOPPED;
#endif

#if MICROKIT_IS_CONFIGURED(MICROKIT_CONFIG_I2C_USE_I2C3)
   DEVICE_I2C3->mcu.Instance = I2C3;
   DEVICE_I2C3->mcu.Init.Timing = 0x00707CBB;
   DEVICE_I2C3->mcu.Init.OwnAddress1 = 0;
   DEVICE_I2C3->mcu.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
   DEVICE_I2C3->mcu.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
   DEVICE_I2C3->mcu.Init.OwnAddress2 = 0;
   DEVICE_I2C3->mcu.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
   DEVICE_I2C3->mcu.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
   DEVICE_I2C3->mcu.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;

   DEVICE_I2C3->state = MKIT_DRIVER_STATE_STOPPED;
#endif
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
void microkit_i2c_start(const MicrokitI2cDevice device, I2cConfig config) {

   ASSERT_NOT_NULL_POINTER(device);

   device->mode = config.mode;

   device->status.isRepeatedStart = false;
   device->status.memoryReadComplete = true;
   device->status.memoryWriteComplete = true;
   device->status.masterReceiveComplete = true;
   device->status.masterTransmitComplete = true;

   device->callbacks.memoryReadCompleteHandler =
       config.memoryReadCompleteHandler;
   device->callbacks.memoryWriteCompleteHandler =
       config.memoryWriteCompleteHandler;
   device->callbacks.masterReceiveCompleteHandler =
       config.masterReceiveCompleteHandler;
   device->callbacks.masterTransmitCompleteHandler =
       config.masterTransmitCompleteHandler;

   if (config.mode == MKIT_I2C_MODE_SLAVE) {
      device->mcu.Init.OwnAddress1 = config.address << 1;
      device->callbacks.slaveByteReceivedHandler =
          config.slaveByteReceivedHandler;
      device->callbacks.slaveByteRequestedHandler =
          config.slaveByteRequestedHandler;
      device->callbacks.slaveTransmissionErrorHandler =
          config.slaveTransmissionErrorHandler;
      device->callbacks.slaveTransmissionStartedHandler =
          config.slaveTransmissionStartedHandler;
      device->callbacks.slaveTransmissionStoppedHandler =
          config.slaveTransmissionStoppedHandler;
   }

   bool ok = false;

   ok = HAL_I2C_Init(&device->mcu) == HAL_OK;
   ASSERT(ok);

   ok = HAL_I2CEx_ConfigAnalogFilter(&device->mcu, I2C_ANALOGFILTER_ENABLE) ==
        HAL_OK;
   ASSERT(ok);

   ok = HAL_I2CEx_ConfigDigitalFilter(&device->mcu, 0) == HAL_OK;
   ASSERT(ok);

   if (config.mode == MKIT_I2C_MODE_SLAVE) {
      rearm_i2c_slave_mode(device);
   }

   device->state = MKIT_DRIVER_STATE_RUNNING;
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
void microkit_i2c_stop(const MicrokitI2cDevice device) {

   ASSERT_NOT_NULL_POINTER(device);

   // TODO: Stop STM32 I2C peripheral

   device->state = MKIT_DRIVER_STATE_STOPPED;
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
   👉 I2C master mode functions
   - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
StatusOrNumber microkit_i2c_receive(
    const MicrokitI2cDevice device, UInt8 deviceAddress,
    UInt8* data, Size dataSize, Bool async) {

   ASSERT_NOT_NULL_POINTER(device);
   ASSERT(device->state == MKIT_DRIVER_STATE_RUNNING);

   if (HAL_I2C_GetState(&device->mcu) != HAL_I2C_STATE_READY) {
      return STATUS_ERROR;
   }

   if (device->status.masterReceiveComplete == false) {
      return STATUS_ERROR;
   }

   HAL_Delay(10); // Without this delay, the transfer is not reliable

   device->status.masterReceiveComplete = false;

   HAL_StatusTypeDef status = HAL_I2C_Master_Receive_IT(
       &device->mcu, deviceAddress << 1, data, dataSize);

   // If we are not in async mode, we need to wait for the transfer to complete
   if (!async) {
      while (!device->status.masterReceiveComplete)
         ;
   }

   return status == HAL_OK ? (int)dataSize : STATUS_ERROR;
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
StatusOrNumber microkit_i2c_transmit(
    const MicrokitI2cDevice device, const UInt8 deviceAddress,
    UInt8* data, Size dataSize, Bool async) {

   ASSERT_NOT_NULL_POINTER(device);
   ASSERT(device->state == MKIT_DRIVER_STATE_RUNNING);

   if (HAL_I2C_GetState(&device->mcu) != HAL_I2C_STATE_READY) {
      return STATUS_ERROR;
   }

   if (device->status.masterTransmitComplete == false) {
      return STATUS_ERROR;
   }

   HAL_Delay(10); // Without this delay, the transfer is not reliable

   device->status.masterTransmitComplete = false;

   HAL_StatusTypeDef status = HAL_I2C_Master_Transmit_IT(
       &device->mcu, deviceAddress << 1, data, dataSize);

   // If we are not in async mode, we need to wait for the transfer to complete
   if (!async) {
      while (!device->status.masterTransmitComplete)
         ;
   }

   return status == HAL_OK ? (int)dataSize : STATUS_ERROR;
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
   👉 I2C master mode memory read/write functions
   - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
StatusOrNumber microkit_i2c_memory_read(
    const MicrokitI2cDevice device, const UInt8 deviceAddress,
    const UInt16 memoryAddress, const UInt16 memoryAddressSize,
    UInt8* data, const Size dataSize, const Bool async) {

   ASSERT_NOT_NULL_POINTER(device);
   ASSERT(device->state == MKIT_DRIVER_STATE_RUNNING);

   if (HAL_I2C_GetState(&device->mcu) != HAL_I2C_STATE_READY) {
      return STATUS_ERROR;
   }

   if (device->status.memoryReadComplete == false) {
      return STATUS_ERROR;
   }

   HAL_Delay(10); // Without this delay, the transfer is not reliable

   device->status.memoryReadComplete = false;

   HAL_StatusTypeDef status = HAL_I2C_Mem_Read_IT(
       &device->mcu, (UInt16)deviceAddress << 1,
       memoryAddress,
       memoryAddressSize,
       data,
       dataSize);

   // If we are not in async mode, we need to wait for the transfer to complete
   if (!async) {
      while (!device->status.memoryReadComplete) {
         // Wait for the transfer to complete
      };
   }

   return status == HAL_OK ? (int)dataSize : STATUS_ERROR;
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
StatusOrNumber microkit_i2c_memory_write(
    const MicrokitI2cDevice device, const UInt8 deviceAddress,
    const UInt16 memoryAddress, const UInt16 memoryAddressSize,
    const UInt8* data, const Size dataSize, const Bool async) {

   ASSERT_NOT_NULL_POINTER(device);
   ASSERT(device->state == MKIT_DRIVER_STATE_RUNNING);

   if (device->status.memoryWriteComplete == false) {
      return STATUS_ERROR;
   }

   HAL_Delay(10); // Without this delay, the transfer is not reliable

   device->status.memoryWriteComplete = false;

   HAL_StatusTypeDef status =
       HAL_I2C_Mem_Write_IT(
           &device->mcu,
           (UInt16)deviceAddress << 1,
           memoryAddress,
           memoryAddressSize,
           (UInt8*)data,
           dataSize);

   // If we are not in async mode, we need to wait for the transfer to complete
   if (!async) {
      while (!device->status.memoryWriteComplete) {
         // Wait for the transfer to complete
      };
   }

   return status == HAL_OK ? (int)dataSize : STATUS_ERROR;
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
   👉 Vendor specific code
   - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
void HAL_I2C_MasterRxCpltCallback(I2C_HandleTypeDef* handle) {

   // I2C master receive completed

   MicrokitI2cDevice device = get_device_from_handle(handle);

   ASSERT_NOT_NULL_POINTER(device);

   if (device->callbacks.masterReceiveCompleteHandler) {
      device->callbacks.masterReceiveCompleteHandler(device);
   }

   device->status.masterReceiveComplete = true;
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
void HAL_I2C_MasterTxCpltCallback(I2C_HandleTypeDef* handle) {

   // I2C master transmit completed

   MicrokitI2cDevice device = get_device_from_handle(handle);

   ASSERT_NOT_NULL_POINTER(device);

   if (device->callbacks.masterTransmitCompleteHandler) {
      device->callbacks.masterTransmitCompleteHandler(device);
   }

   device->status.masterTransmitComplete = true;
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
void HAL_I2C_MemRxCpltCallback(I2C_HandleTypeDef* handle) {

   // I2C memory read completed

   MicrokitI2cDevice device = get_device_from_handle(handle);

   ASSERT_NOT_NULL_POINTER(device);

   if (device->callbacks.memoryReadCompleteHandler) {
      device->callbacks.memoryReadCompleteHandler(device);
   }

   device->status.memoryReadComplete = true;
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
void HAL_I2C_MemTxCpltCallback(I2C_HandleTypeDef* handle) {

   // I2C memory write completed

   MicrokitI2cDevice device = get_device_from_handle(handle);

   ASSERT_NOT_NULL_POINTER(device);

   if (device->callbacks.memoryWriteCompleteHandler) {
      device->callbacks.memoryWriteCompleteHandler(device);
   }

   device->status.memoryWriteComplete = true;
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
#if MICROKIT_IS_CONFIGURED(CONFIG_I2C_USE_SLAVE_MODE) // 👉 Slave mode code
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
void HAL_I2C_AddrCallback(I2C_HandleTypeDef* handle, uint8_t TransferDirection,
                          uint16_t AddrMatchCode) {
   // DEBUG_PRINT("I2C ADDRESS MATCH\n");

   // I2C START detected / Transmission has started

   MicrokitI2cDevice device = get_device_from_handle(handle);

   ASSERT_NOT_NULL_POINTER(device->callbacks.slaveTransmissionStartedHandler);
   ASSERT_NOT_NULL_POINTER(device->callbacks.slaveTransmissionErrorHandler);

   if (TransferDirection == I2C_DIRECTION_TRANSMIT) {
      // DEBUG_PRINT("I2C TX\n");

      // Master wants to transmit data. So we - the slave - receive it

      device->callbacks.slaveTransmissionStartedHandler(
          device, MKIT_I2C_DIRECTION_WRITE, device->status.isRepeatedStart);

      // I2C_FIRST_FRAME means SLAVE receives the first byte of a sequence.
      if (HAL_I2C_Slave_Seq_Receive_IT(handle, &device->slave.rxByte, 1,
                                       I2C_FIRST_FRAME) != HAL_OK) {
         //     device->callbacks.slaveByteReceivedHandler(device, &byte);
         // }
         // else {
         device->callbacks.slaveTransmissionErrorHandler(device,
                                                         MKIT_I2C_ERROR);
      }
   } else {
      // DEBUG_PRINT("I2C RX\n");

      // Master wants to receive data. So we - the slave - transmit it

      device->callbacks.slaveTransmissionStartedHandler(
          device, MKIT_I2C_DIRECTION_READ, device->status.isRepeatedStart);

      device->callbacks.slaveByteRequestedHandler(device,
                                                  &device->slave.txByte);

      // I2C_FIRST_FRAME means SLAVE transmits the first byte of a sequence.
      if (HAL_I2C_Slave_Seq_Transmit_IT(handle, &device->slave.txByte, 1,
                                        I2C_FIRST_FRAME) != HAL_OK) {
         device->callbacks.slaveTransmissionErrorHandler(device,
                                                         MKIT_I2C_ERROR);
      }
   }

   // The isRepeatedStart is set to true on first START condition so it is true
   // on consecutive START conditions. It will be reset to false on STOP
   // condition.
   device->status.isRepeatedStart = true;
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
void HAL_I2C_SlaveTxCpltCallback(I2C_HandleTypeDef* handle) {
   // DEBUG_PRINT("I2C SLAVE TX COMPLETE\n");

   MicrokitI2cDevice device = get_device_from_handle(handle);

   // static uint8 byte = 0x00;
   device->callbacks.slaveByteRequestedHandler(device, &device->slave.txByte);
   // DEBUG_PRINT("I2C SLAVE TX COMPLETE > 0x%02x\n", device->slave.txByte);

   // I2C_NEXT_FRAME means SLAVE is ready to transmit another byte.
   HAL_I2C_Slave_Seq_Transmit_IT(handle, &device->slave.txByte, 1,
                                 I2C_NEXT_FRAME);
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
void HAL_I2C_SlaveRxCpltCallback(I2C_HandleTypeDef* handle) {
   // DEBUG_PRINT("I2C SLAVE RX COMPLETE\n");

   MicrokitI2cDevice device = get_device_from_handle(handle);

   // static uint8 byte = 0x00;
   if (HAL_I2C_Slave_Seq_Receive_IT(handle, &device->slave.rxByte, 1,
                                    I2C_NEXT_FRAME) == HAL_OK) {
      device->callbacks.slaveByteReceivedHandler(device, &device->slave.rxByte);
   } else {
      device->callbacks.slaveTransmissionErrorHandler(device, MKIT_I2C_ERROR);
   }

   // TODO: Check if we need to handle the LAST_FRAME

   // if (rxcount < RX_BUFFER_SIZE) {
   // 	if (rxcount == RX_BUFFER_SIZE-1)
   // 	{
   // 		HAL_I2C_Slave_Seq_Receive_IT(handle, &byte, 1, I2C_LAST_FRAME);
   // 	}
   // 	else
   // 	{
   // 		HAL_I2C_Slave_Seq_Receive_IT(handle, &byte, 1, I2C_NEXT_FRAME);
   // 	}
   // }

   // if (rxcount == RX_BUFFER_SIZE)
   // {
   // 	// Invoke processing callback
   // }
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
void HAL_I2C_ListenCpltCallback(I2C_HandleTypeDef* handle) {
   // DEBUG_PRINT("I2C LISTEN COMPLETE\n");

   // I2C STOP detected / Transmission is complete

   MicrokitI2cDevice device = get_device_from_handle(handle);

   device->callbacks.slaveTransmissionStoppedHandler(device);

   rearm_i2c_slave_mode(device);
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
void HAL_I2C_ErrorCallback(I2C_HandleTypeDef* handle) {
   // DEBUG_PRINT("I2C ERROR\n");

   MicrokitI2cDevice device = get_device_from_handle(handle);

   // TODO: Implement error statistics

   UInt32 errorcode = HAL_I2C_GetError(handle);

   if (errorcode == HAL_I2C_ERROR_BERR) {
      // BERR error

      // DEBUG_PRINT("I2C BUS ERROR\n");

      HAL_I2C_DeInit(handle);
      HAL_I2C_Init(handle);
   } else if (errorcode == HAL_I2C_ERROR_AF) {
      // AF error

      // DEBUG_PRINT("I2C ACK ERROR\n");

      // ACK Failure is raised when master sends a STOP before buffer is full.
      // Instead an error, we treat it as a regular STOP condition.
      // We don't have to call the slaveTransmissionStoppedHandler(device)
      // callback because the listen complete handler is called which then calls
      // the slaveTransmissionStoppedHandler(device) callback
   } else {
      Microkit.logger.log(MKIT_LOG_LEVEL_WARN, "UNHANDLED I2C ERROR [%d]", errorcode);
   }

   if (device->mode == MKIT_I2C_MODE_MASTER) {
      if (device->callbacks.masterReceiveCompleteHandler) {
         device->callbacks.masterReceiveCompleteHandler(device);
      }

      if (device->callbacks.masterTransmitCompleteHandler) {
         device->callbacks.masterTransmitCompleteHandler(device);
      }

      if (device->callbacks.memoryReadCompleteHandler) {
         device->callbacks.memoryReadCompleteHandler(device);
      }

      if (device->callbacks.memoryWriteCompleteHandler) {
         device->callbacks.memoryWriteCompleteHandler(device);
      }

      device->status.masterReceiveComplete = true;
      device->status.masterTransmitComplete = true;
      device->status.memoryReadComplete = true;
      device->status.memoryWriteComplete = true;
   }

   if (device->mode == MKIT_I2C_MODE_SLAVE) {
      rearm_i2c_slave_mode(device);
   }
}
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
#endif // CONFIG_I2C_USE_SLAVE_MODE
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
#if MICROKIT_IS_CONFIGURED(MICROKIT_CONFIG_I2C_USE_I2C1)
void I2C1_EV_IRQHandler(void) { HAL_I2C_EV_IRQHandler(&DEVICE_I2C1->mcu); }
#endif

#if MICROKIT_IS_CONFIGURED(MICROKIT_CONFIG_I2C_USE_I2C2)
void I2C2_EV_IRQHandler(void) { HAL_I2C_EV_IRQHandler(&DEVICE_I2C2->mcu); }
#endif

#if MICROKIT_IS_CONFIGURED(MICROKIT_CONFIG_I2C_USE_I2C3)
void I2C3_EV_IRQHandler(void) { HAL_I2C_EV_IRQHandler(&DEVICE_I2C3->mcu); }
#endif

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
#if MICROKIT_IS_CONFIGURED(MICROKIT_CONFIG_I2C_USE_I2C1)
void I2C1_ER_IRQHandler(void) { HAL_I2C_ER_IRQHandler(&DEVICE_I2C1->mcu); }
#endif

#if MICROKIT_IS_CONFIGURED(MICROKIT_CONFIG_I2C_USE_I2C2)
void I2C2_ER_IRQHandler(void) { HAL_I2C_ER_IRQHandler(&DEVICE_I2C2->mcu); }
#endif

#if MICROKIT_IS_CONFIGURED(MICROKIT_CONFIG_I2C_USE_I2C3)
void I2C3_ER_IRQHandler(void) { HAL_I2C_ER_IRQHandler(&DEVICE_I2C3->mcu); }
#endif
