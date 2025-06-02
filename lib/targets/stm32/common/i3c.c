#include "libs/microkit/lib/assert.h"
#include "libs/microkit/lib/core.h"
#include "libs/microkit/lib/debug.h"
#include "libs/microkit/lib/microkit.h"
#include "libs/microkit/lib/platform/drivers/i3c.h"
#include "libs/microkit/lib/utils/math.h"
#include "microkit/config/i3c.h"

#include "stm32h5xx_hal.h"

#include <string.h>

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
   👉 Constants
   - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
#define MICROKIT_CONFIG_I3C2_MAX_TARGET_DESCRIPTORS 4 // STM32H533RE support up to 4 targets

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
   👉 Types
   - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
// typedef struct {
//    char* TARGET_NAME;           /*!< Marketing Target reference */
//    uint32_t TARGET_ID;          /*!< Target Identifier on the Bus */
//    uint64_t TARGET_BCR_DCR_PID; /*!< Concatenation value of PID, BCR and DCR of the target */
//    uint8_t STATIC_ADDR;         /*!< Static Address of the target, value found in the datasheet of the device */
//    uint8_t DYNAMIC_ADDR;        /*!< Dynamic Address of the target preset by software/application */
// } TargetDesc_TypeDef;

typedef enum {
   MKIT_I3C_BUS_STATE_RESET,
   MKIT_I3C_BUS_STATE_READY,
   MKIT_I3C_BUS_STATE_LISTENING,
   MKIT_I3C_BUS_STATE_RECEIVING,
   MKIT_I3C_BUS_STATE_TRANSMITTING,
   MKIT_I3C_BUS_STATE_DYN_ADDR_ASSIGNMENT_RUNNING,
   MKIT_I3C_BUS_STATE_DYN_ADDR_ASSIGNMENT_FINISHED,
   MKIT_I3C_BUS_STATE_ERROR,
} I3cBusState;

// typedef struct {
//    bool isRepeatedStart;
//    bool memoryReadComplete;
//    bool memoryWriteComplete;
//    bool masterReceiveComplete;
//    bool masterTransmitComplete;
// } I2cStatus;

typedef struct {
   // I2cMasterMemoryReadCompleteHandler memoryReadCompleteHandler;
   // I2cMasterMemoryWriteCompleteHandler memoryWriteCompleteHandler;
   // I2cMasterReceiveCompleteHandler masterReceiveCompleteHandler;
   // I2cMasterTransmitCompleteHandler masterTransmitCompleteHandler;
   // I2cSlaveTransmissionStartedHandler slaveTransmissionStartedHandler;
   // I2cSlaveTransmissionStoppedHandler slaveTransmissionStoppedHandler;
   // I2cSlaveTransmissionErrorHandler slaveTransmissionErrorHandler;
   // I2cSlaveByteReceivedHandler slaveByteReceivedHandler;
   // I2cSlaveByteRequestedHandler slaveByteRequestedHandler;
} I3cCallbacks;

// typedef struct {
//    UInt8 rxByte;
//    UInt8 txByte;
// } I2cSlaveState;

// typedef I3cTargetDeviceConfiguration I3cTargetDeviceConfiguration;

/**
 * @brief I3C device.
 */
struct I3cDeviceObject {
   DriverState state;
   I3cBusState busState;
   I3cCallbacks callbacks;
   I3cTargetDescriptor targetDescriptors[MICROKIT_CONFIG_I3C2_MAX_TARGET_DESCRIPTORS];
   Size targetDescriptorsCount;
   // I3cTargetDeviceConfiguration targetConfigurations[MICROKIT_CONFIG_I3C2_MAX_TARGET_DESCRIPTORS];
   Size targetCount;
   //    I2cMode mode;
   // I3cStatus status;
   //    I2cSlaveState slave;
   bool controllerMemoryWriteComplete;
   UInt8 receiveBuffer[1];
   UInt8 transmitBuffer[1];
   bool targetAddressAssignmentComplete;
   bool targetReceiveComplete;
   bool targetTransmitComplete;
   I3C_HandleTypeDef mcu;
   I3C_XferTypeDef mcuContextBuffers;
};

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
   👉 Configuration
   - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
#if MICROKIT_IS_CONFIGURED(MICROKIT_CONFIG_I3C_USE_I3C1)
struct I3cDeviceObject DEVICE_I3C1_INSTANCE = {
    .busState = MKIT_I3C_BUS_STATE_RESET,
    .state = MKIT_DRIVER_STATE_UNINITIALIZED,
    .targetCount = 0,
    .controllerMemoryWriteComplete = true,
    .targetDescriptorsCount = 0,
    .targetAddressAssignmentComplete = false,
    .targetReceiveComplete = false,
    .targetTransmitComplete = false,
    .mcu = {0},
};
MicrokitI3cDevice DEVICE_I3C1 = &DEVICE_I3C1_INSTANCE;
#endif // MICROKIT_CONFIG_I3C_USE_I3C1

#if MICROKIT_IS_CONFIGURED(MICROKIT_CONFIG_I3C_USE_I3C2)
struct I3cDeviceObject DEVICE_I3C2_INSTANCE = {
    .busState = MKIT_I3C_BUS_STATE_RESET,
    .state = MKIT_DRIVER_STATE_UNINITIALIZED,
    .targetCount = 0,
    .controllerMemoryWriteComplete = true,
    .targetDescriptorsCount = 0,
    .targetAddressAssignmentComplete = false,
    .targetReceiveComplete = false,
    .targetTransmitComplete = false,
    .mcu = {0},
};
MicrokitI3cDevice DEVICE_I3C2 = &DEVICE_I3C2_INSTANCE;
#endif // MICROKIT_CONFIG_I3C_USE_I3C2

#if MICROKIT_IS_CONFIGURED(MICROKIT_CONFIG_I3C_USE_I3C3)
struct I3cDeviceObject DEVICE_I3C3_INSTANCE = {
    .busState = MKIT_I3C_BUS_STATE_RESET,
    .state = MKIT_DRIVER_STATE_UNINITIALIZED,
    .targetCount = 0,
    .controllerMemoryWriteComplete = true,
    .targetDescriptorsCount = 0,
    .targetAddressAssignmentComplete = false,
    .targetReceiveComplete = false,
    .targetTransmitComplete = false,
    .mcu = {0},
};
MicrokitI3cDevice DEVICE_I3C3 = &DEVICE_I3C3_INSTANCE;
#endif // MICROKIT_CONFIG_I3C_USE_I3C3

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
   👉 Private functions
   - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

static inline MicrokitI3cDevice private_get_device_from_handle(I3C_HandleTypeDef* handle) {
#if MICROKIT_IS_CONFIGURED(MICROKIT_CONFIG_I3C_USE_I3C1)
   if (handle == &DEVICE_I3C1->mcu) {
      return DEVICE_I3C1;
   }
#endif
#if MICROKIT_IS_CONFIGURED(MICROKIT_CONFIG_I3C_USE_I3C2)
   if (handle == &DEVICE_I3C2->mcu) {
      return DEVICE_I3C2;
   }
#endif
#if MICROKIT_IS_CONFIGURED(MICROKIT_CONFIG_I3C_USE_I3C3)
   if (handle == &DEVICE_I3C3->mcu) {
      return DEVICE_I3C3;
   }
#endif

   ASSERT_NOT_REACHED;

   return 0;
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
void private_controller_start_dynamic_address_assignment(MicrokitI3cDevice device) {

   device->busState = MKIT_I3C_BUS_STATE_DYN_ADDR_ASSIGNMENT_RUNNING;

   if (HAL_I3C_Ctrl_DynAddrAssign_IT(&device->mcu, I3C_ONLY_ENTDAA) != HAL_OK) {
      device->busState = MKIT_I3C_BUS_STATE_ERROR;
   }
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
void private_target_activate_notifications(MicrokitI3cDevice device) {
   bool ok = HAL_I3C_ActivateNotification(&device->mcu, NULL, (HAL_I3C_IT_DAUPDIE | HAL_I3C_IT_FCIE)) == HAL_OK;
   ASSERT(ok);
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
void private_target_start_listening(MicrokitI3cDevice device) {

   device->mcuContextBuffers.RxBuf.pBuffer = device->receiveBuffer;
   device->mcuContextBuffers.RxBuf.Size = ARRAY_SIZE(device->receiveBuffer);

   bool ok = HAL_I3C_Tgt_Receive_IT(&device->mcu, &device->mcuContextBuffers) == HAL_OK;
   ASSERT(ok);

   device->busState = MKIT_I3C_BUS_STATE_LISTENING;

   Microkit.logger.log(MKIT_LOG_LEVEL_TRACE, "[I3C] Start listening");
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
   👉 Lifecycle functions
   - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
void microkit_i3c_init(void) {

#if MICROKIT_IS_CONFIGURED(MICROKIT_CONFIG_I3C_USE_I3C1)
   // TBD
#endif

#if MICROKIT_IS_CONFIGURED(MICROKIT_CONFIG_I3C_USE_I3C2)
#if MICROKIT_CONFIG_I3C2_MODE == MICROKIT_I3C_CONTROLLER_MODE
   DEVICE_I3C2->mcu.Instance = I3C2;
   DEVICE_I3C2->mcu.Mode = HAL_I3C_MODE_CONTROLLER;
   DEVICE_I3C2->mcu.Init.CtrlBusCharacteristic.SDAHoldTime = HAL_I3C_SDA_HOLD_TIME_0_5;
   DEVICE_I3C2->mcu.Init.CtrlBusCharacteristic.WaitTime = HAL_I3C_OWN_ACTIVITY_STATE_0;
   DEVICE_I3C2->mcu.Init.CtrlBusCharacteristic.SCLPPLowDuration = 0x27;
   DEVICE_I3C2->mcu.Init.CtrlBusCharacteristic.SCLI3CHighDuration = 0x27;
   DEVICE_I3C2->mcu.Init.CtrlBusCharacteristic.SCLODLowDuration = 0x27;
   DEVICE_I3C2->mcu.Init.CtrlBusCharacteristic.SCLI2CHighDuration = 0x00;
   DEVICE_I3C2->mcu.Init.CtrlBusCharacteristic.BusFreeDuration = 0x06;
   DEVICE_I3C2->mcu.Init.CtrlBusCharacteristic.BusIdleDuration = 0x1e;
#elif MICROKIT_CONFIG_I3C2_MODE == MICROKIT_I3C_TARGET_MODE
   DEVICE_I3C2->mcu.Instance = I3C2;
   DEVICE_I3C2->mcu.Mode = HAL_I3C_MODE_TARGET;
   DEVICE_I3C2->mcu.Init.TgtBusCharacteristic.BusAvailableDuration = 0x1e;
#endif // MICROKIT_CONFIG_I3C2_MODE == MICROKIT_I3C_TARGET_MODE
   DEVICE_I3C2->state = MKIT_DRIVER_STATE_STOPPED;
#endif // MICROKIT_CONFIG_I3C_USE_I3C2

#if MICROKIT_IS_CONFIGURED(MICROKIT_CONFIG_I3C_USE_I3C3)
   // TBD
#endif
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
void microkit_i3c_start(const MicrokitI3cDevice device, I3cConfig config) {

   ASSERT_NOT_NULL_POINTER(device);

   bool ok = false;

#if MICROKIT_CONFIG_I3C2_MODE == MICROKIT_I3C_CONTROLLER_MODE
   ASSERT_NOT_NULL_POINTER(config.targetDescriptors);

   device->targetDescriptorsCount = min(config.targetDescriptorsCount, MICROKIT_CONFIG_I3C2_MAX_TARGET_DESCRIPTORS);
   for (Size i = 0; i < device->targetDescriptorsCount; i++) {
      device->targetDescriptors[i] = config.targetDescriptors[i];
   }

   ok = HAL_I3C_Init(&DEVICE_I3C2->mcu) == HAL_OK;
   ASSERT(ok);

   I3C_FifoConfTypeDef fifoConfig = {0};
   fifoConfig.RxFifoThreshold = HAL_I3C_RXFIFO_THRESHOLD_1_4;
   fifoConfig.TxFifoThreshold = HAL_I3C_TXFIFO_THRESHOLD_1_4;
   fifoConfig.ControlFifo = HAL_I3C_CONTROLFIFO_DISABLE;
   fifoConfig.StatusFifo = HAL_I3C_STATUSFIFO_DISABLE;

   ok = HAL_I3C_SetConfigFifo(&DEVICE_I3C2->mcu, &fifoConfig) == HAL_OK;
   ASSERT(ok);

   /** Configure controller
    */
   I3C_CtrlConfTypeDef controllerConfig = {0};
   controllerConfig.DynamicAddr = 0;
   controllerConfig.StallTime = 0x00;
   controllerConfig.HotJoinAllowed = DISABLE;
   controllerConfig.ACKStallState = DISABLE;
   controllerConfig.CCCStallState = DISABLE;
   controllerConfig.TxStallState = DISABLE;
   controllerConfig.RxStallState = DISABLE;
   controllerConfig.HighKeeperSDA = DISABLE;

   ok = HAL_I3C_Ctrl_Config(&DEVICE_I3C2->mcu, &controllerConfig) == HAL_OK;
   ASSERT(ok);

   private_controller_start_dynamic_address_assignment(device);
#elif MICROKIT_CONFIG_I3C2_MODE == MICROKIT_I3C_TARGET_MODE
   ok = HAL_I3C_Init(&DEVICE_I3C2->mcu) == HAL_OK;
   ASSERT(ok);

   I3C_FifoConfTypeDef fifoConfig = {0};
   fifoConfig.RxFifoThreshold = HAL_I3C_RXFIFO_THRESHOLD_1_4;
   fifoConfig.TxFifoThreshold = HAL_I3C_TXFIFO_THRESHOLD_1_4;
   fifoConfig.ControlFifo = HAL_I3C_CONTROLFIFO_DISABLE;
   fifoConfig.StatusFifo = HAL_I3C_STATUSFIFO_DISABLE;

   ok = HAL_I3C_SetConfigFifo(&DEVICE_I3C2->mcu, &fifoConfig) == HAL_OK;
   ASSERT(ok);

   I3C_TgtConfTypeDef targetConfig = {0};
   targetConfig.Identifier = 0;
   targetConfig.MIPIIdentifier = 0;
   targetConfig.CtrlRoleRequest = DISABLE;
   targetConfig.HotJoinRequest = DISABLE;
   targetConfig.IBIRequest = DISABLE;
   targetConfig.IBIPayload = DISABLE;
   targetConfig.IBIPayloadSize = HAL_I3C_PAYLOAD_1_BYTE;
   targetConfig.MaxReadDataSize = 0;
   targetConfig.MaxWriteDataSize = 0;
   targetConfig.CtrlCapability = DISABLE;
   targetConfig.GroupAddrCapability = DISABLE;
   targetConfig.DataTurnAroundDuration = HAL_I3C_TURNAROUND_TIME_TSCO_LESS_12NS;
   targetConfig.MaxReadTurnAround = 0;
   targetConfig.MaxDataSpeed = HAL_I3C_GETMXDS_FORMAT_1;
   targetConfig.MaxSpeedLimitation = DISABLE;
   targetConfig.HandOffActivityState = HAL_I3C_HANDOFF_ACTIVITY_STATE_0;
   targetConfig.HandOffDelay = DISABLE;
   targetConfig.PendingReadMDB = DISABLE;

   ok = HAL_I3C_Tgt_Config(&DEVICE_I3C2->mcu, &targetConfig) == HAL_OK;
   ASSERT(ok);

   private_target_activate_notifications(device);
#endif

   device->controllerMemoryWriteComplete = true;
   device->state = MKIT_DRIVER_STATE_RUNNING;
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
void microkit_i3c_stop(const MicrokitI3cDevice device) {

   ASSERT_NOT_NULL_POINTER(device);

   // TODO: Stop STM32 I3C peripheral

   device->state = MKIT_DRIVER_STATE_STOPPED;
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
void microkit_i3c_process(const MicrokitI3cDevice device) {

   if (HAL_I3C_GetState(&device->mcu) != HAL_I3C_STATE_READY) {
      // Wait until peripheral is ready again
   } else if (device->busState == MKIT_I3C_BUS_STATE_DYN_ADDR_ASSIGNMENT_FINISHED) {
      for (Size index = 0; index < device->targetCount; index++) {
         I3C_DeviceConfTypeDef deviceConfiguration = {0};

         deviceConfiguration.DeviceIndex = (index + 1);
         deviceConfiguration.TargetDynamicAddr = device->targetDescriptors[index].dynamicAddress;
         deviceConfiguration.IBIAck = __HAL_I3C_GET_IBI_CAPABLE(__HAL_I3C_GET_BCR(device->targetDescriptors[index].targetBcrDcrPid));
         deviceConfiguration.IBIPayload = __HAL_I3C_GET_IBI_PAYLOAD(__HAL_I3C_GET_BCR(device->targetDescriptors[index].targetBcrDcrPid));
         deviceConfiguration.CtrlRoleReqAck = __HAL_I3C_GET_CR_CAPABLE(__HAL_I3C_GET_BCR(device->targetDescriptors[index].targetBcrDcrPid));
         deviceConfiguration.CtrlStopTransfer = DISABLE;

         if (HAL_I3C_Ctrl_ConfigBusDevices(&device->mcu, &deviceConfiguration, 1U) != HAL_OK) {
            device->busState = MKIT_I3C_BUS_STATE_ERROR;
         }
      }

      device->busState = MKIT_I3C_BUS_STATE_READY;
   }

   if (device->targetAddressAssignmentComplete) {
      device->targetAddressAssignmentComplete = false;

      Microkit.logger.log(MKIT_LOG_LEVEL_TRACE, "[I3C] Address assigned");

      private_target_start_listening(device);
   }

   if (device->targetReceiveComplete) {
      device->targetReceiveComplete = false;

      Microkit.logger.log(MKIT_LOG_LEVEL_TRACE, "[I3C] Received: 0x%02X", device->receiveBuffer[0]);

      private_target_start_listening(device);
   }

   if (device->targetTransmitComplete) {
      device->targetTransmitComplete = false;

      Microkit.logger.log(MKIT_LOG_LEVEL_TRACE, "[I3C] Transmitted 0x%02X");

      private_target_start_listening(device);
   }
}

// /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//    👉 I2C master mode functions
//    - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
// StatusOrNumber microkit_i2c_receive(
//     const MicrokitI2cDevice device, UInt8 deviceAddress,
//     UInt8* data, Size dataSize, Bool async) {

//    ASSERT_NOT_NULL_POINTER(device);
//    ASSERT(device->state == MKIT_DRIVER_STATE_RUNNING);

//    if (HAL_I2C_GetState(&device->mcu) != HAL_I2C_STATE_READY) {
//       return STATUS_ERROR;
//    }

//    if (device->status.masterReceiveComplete == false) {
//       return STATUS_ERROR;
//    }

//    HAL_Delay(10); // Without this delay, the transfer is not reliable

//    device->status.masterReceiveComplete = false;

//    HAL_StatusTypeDef status = HAL_I2C_Master_Receive_IT(
//        &device->mcu, deviceAddress << 1, data, dataSize);

//    // If we are not in async mode, we need to wait for the transfer to complete
//    if (!async) {
//       while (!device->status.masterReceiveComplete)
//          ;
//    }

//    return status == HAL_OK ? (int)dataSize : STATUS_ERROR;
// }

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
StatusOrNumber microkit_i3c_transmit(
    const MicrokitI3cDevice device, const UInt8 deviceAddress,
    UInt8* data, Size dataSize, Bool async) {

   ASSERT_NOT_NULL_POINTER(device);
   ASSERT(device->state == MKIT_DRIVER_STATE_RUNNING);

   if (device->busState != MKIT_I3C_BUS_STATE_READY) {
      return STATUS_ERROR;
   }

   HAL_Delay(10); // Without this delay, the transfer is not reliable

   device->busState = MKIT_I3C_BUS_STATE_TRANSMITTING;

   I3C_PrivateTypeDef transferDescriptor =
       {deviceAddress, {data, dataSize}, {NULL, 0}, HAL_I3C_DIRECTION_WRITE};

   UInt32 controlBuffer[1];
   I3C_XferTypeDef context;
   context.CtrlBuf.pBuffer = controlBuffer;
   context.CtrlBuf.Size = sizeof(controlBuffer);
   context.TxBuf.pBuffer = data;
   context.TxBuf.Size = dataSize;

   HAL_StatusTypeDef status = HAL_I3C_AddDescToFrame(&device->mcu, NULL, &transferDescriptor, &context, 1, I3C_PRIVATE_WITH_ARB_STOP);

   if (status != HAL_OK) {
      device->busState = MKIT_I3C_BUS_STATE_READY;
      return STATUS_ERROR;
   }

   status = HAL_I3C_Ctrl_Transmit_IT(&device->mcu, &context);

   //    // If we are not in async mode, we need to wait for the transfer to complete
   // if (!async) {
   //    while (!device->status.masterTransmitComplete)
   //       ;
   // }

   device->busState = MKIT_I3C_BUS_STATE_READY;

   return status == HAL_OK ? (int)dataSize : STATUS_ERROR;
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
bool microkit_i3c_is_i3c_target_ready(const MicrokitI3cDevice device, const UInt8 deviceAddress) {

   ASSERT_NOT_NULL_POINTER(device);

   return HAL_I3C_Ctrl_IsDeviceI3C_Ready(&device->mcu, deviceAddress, 1, 1000) == HAL_OK;
}

// /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//    👉 I2C master mode memory read/write functions
//    - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
// StatusOrNumber microkit_i2c_memory_read(
//     const MicrokitI2cDevice device, const UInt8 deviceAddress,
//     const UInt16 memoryAddress, const UInt16 memoryAddressSize,
//     UInt8* data, const Size dataSize, const Bool async) {

//    ASSERT_NOT_NULL_POINTER(device);
//    ASSERT(device->state == MKIT_DRIVER_STATE_RUNNING);

//    if (HAL_I2C_GetState(&device->mcu) != HAL_I2C_STATE_READY) {
//       return STATUS_ERROR;
//    }

//    if (device->status.memoryReadComplete == false) {
//       return STATUS_ERROR;
//    }

//    HAL_Delay(10); // Without this delay, the transfer is not reliable

//    device->status.memoryReadComplete = false;

//    HAL_StatusTypeDef status = HAL_I2C_Mem_Read_IT(
//        &device->mcu, (UInt16)deviceAddress << 1,
//        memoryAddress,
//        memoryAddressSize,
//        data,
//        dataSize);

//    // If we are not in async mode, we need to wait for the transfer to complete
//    if (!async) {
//       while (!device->status.memoryReadComplete) {
//          // Wait for the transfer to complete
//       };
//    }

//    return status == HAL_OK ? (int)dataSize : STATUS_ERROR;
// }
#define MICROKIT_I3C_ADDRESSBUFFER_SIZE 4
#define MICROKIT_I3C_DATABUFFER_SIZE 16

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
StatusOrNumber microkit_i3c_memory_write(
    const MicrokitI3cDevice device, const UInt8 deviceAddress,
    const UInt16 memoryAddress, const UInt16 memoryAddressSize,
    const UInt8* data, const Size dataSize, const Bool async) {

   ASSERT_NOT_NULL_POINTER(device);
   ASSERT(device->state == MKIT_DRIVER_STATE_RUNNING);

   if (device->controllerMemoryWriteComplete == false) {
      return STATUS_ERROR;
   }

   HAL_Delay(10); // Without this delay, the transfer is not reliable

   device->controllerMemoryWriteComplete = false;

   HAL_StatusTypeDef status;

   // 1. Create a transfer descriptor for the memory address
   UInt8 addressBuffer[MICROKIT_I3C_ADDRESSBUFFER_SIZE];
   Size addressBufferSize = min(MICROKIT_I3C_ADDRESSBUFFER_SIZE, memoryAddressSize);

   memcpy(addressBuffer, &memoryAddress, memoryAddressSize); // Copy either 1 or 2 bytes

   I3C_PrivateTypeDef addressTransfer = {
       .Direction = HAL_I3C_DIRECTION_WRITE,
       .TargetAddr = deviceAddress,
       .TxBuf = {addressBuffer, addressBufferSize},
       .RxBuf = {NULL, 0},
   };

   // 2. Create a transfer descriptor for the data
   UInt8 dataBuffer[MICROKIT_I3C_DATABUFFER_SIZE];
   Size dataBufferSize = min(MICROKIT_I3C_DATABUFFER_SIZE, dataSize);

   memcpy(dataBuffer, data, dataBufferSize); // Copy either 1 or 2 bytes

   I3C_PrivateTypeDef dataTransfer = {
       .Direction = HAL_I3C_DIRECTION_WRITE,
       .TargetAddr = deviceAddress,
       .TxBuf = {dataBuffer, dataBufferSize},
       .RxBuf = {NULL, 0},
   };

   // 3. Create a transfer descriptor for the whole transfer
   I3C_PrivateTypeDef transferDescriptor[2] =
       {
           addressTransfer,
           dataTransfer,
       };

   // Create the whole transfer descriptor
   UInt32 controlBuffer[2];
   UInt8 txBuffer[addressBufferSize + dataBufferSize];

   I3C_XferTypeDef transfer = {
       .CtrlBuf = {controlBuffer, 2},
       .StatusBuf = {NULL, 0},
       .TxBuf = {txBuffer, addressBufferSize + dataBufferSize}};

   status = HAL_I3C_AddDescToFrame(
       &device->mcu,
       NULL,
       transferDescriptor,
       &transfer,
       transfer.CtrlBuf.Size,
       I3C_PRIVATE_WITH_ARB_RESTART);

   // Transmit the data
   status = HAL_I3C_Ctrl_Transmit_IT(&device->mcu, &transfer);
   return status;

   // If we are not in async mode, we need to wait for the transfer to complete
   if (!async) {
      while (!device->controllerMemoryWriteComplete) {
         // Wait for the transfer to complete
      };
   }

   return status == HAL_OK ? (int)dataSize : STATUS_ERROR;
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
   👉 Vendor specific code
   - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
void HAL_I3C_TgtReqDynamicAddrCallback(I3C_HandleTypeDef* handle, uint64_t targetPayload) {

   // Called on controller when a target responded to a dynamic address request

   MicrokitI3cDevice device = private_get_device_from_handle(handle);

   ASSERT_NOT_NULL_POINTER(device);

   /* Update Payload on aTargetDesc */
   device->targetDescriptors[device->targetCount].targetBcrDcrPid = targetPayload;
   // aTargetDesc[uwTargetCount]->TARGET_BCR_DCR_PID = targetPayload;

   // Send associated dynamic address to target
   HAL_I3C_Ctrl_SetDynAddr(handle, device->targetDescriptors[device->targetCount].dynamicAddress);

   device->targetCount++;
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
void HAL_I3C_CtrlDAACpltCallback(I3C_HandleTypeDef* handle) {

   // Called on controller when all targets have been assigned a dynamic address

   MicrokitI3cDevice device = private_get_device_from_handle(handle);

   device->busState = MKIT_I3C_BUS_STATE_DYN_ADDR_ASSIGNMENT_FINISHED;
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
void HAL_I3C_CtrlTxCpltCallback(I3C_HandleTypeDef* handle) {

   MicrokitI3cDevice device = private_get_device_from_handle(handle);

   device->controllerMemoryWriteComplete = true;
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
void HAL_I3C_TgtRxCpltCallback(I3C_HandleTypeDef* handle) {

   // Called on target when all data of a transmission is received

   MicrokitI3cDevice device = private_get_device_from_handle(handle);

   device->targetReceiveComplete = true;
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
void HAL_I3C_TgtTxCpltCallback(I3C_HandleTypeDef* handle) {

   // Called when target has transmitted all data of a transmission

   MicrokitI3cDevice device = private_get_device_from_handle(handle);

   device->targetTransmitComplete = true;
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
void HAL_I3C_NotifyCallback(I3C_HandleTypeDef* handle, UInt32 eventId) {

   MicrokitI3cDevice device = private_get_device_from_handle(handle);

   if ((eventId & EVENT_ID_DAU) == EVENT_ID_DAU) {
      device->busState = MKIT_I3C_BUS_STATE_READY;

      device->targetAddressAssignmentComplete = true;
   }
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
void HAL_I3C_ErrorCallback(I3C_HandleTypeDef* handle) {

   MicrokitI3cDevice device = private_get_device_from_handle(handle);
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
#if MICROKIT_IS_CONFIGURED(MICROKIT_CONFIG_I3C_USE_I3C1)
void I3C1_EV_IRQHandler(void) {
   HAL_I3C_EV_IRQHandler(&DEVICE_I3C1->mcu);
}
#endif

#if MICROKIT_IS_CONFIGURED(MICROKIT_CONFIG_I3C_USE_I3C2)
void I3C2_EV_IRQHandler(void) {
   HAL_I3C_EV_IRQHandler(&DEVICE_I3C2->mcu);
}
#endif

#if MICROKIT_IS_CONFIGURED(MICROKIT_CONFIG_I3C_USE_I3C3)
void I3C3_EV_IRQHandler(void) {
   HAL_I3C_EV_IRQHandler(&DEVICE_I3C3->mcu);
}
#endif

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
#if MICROKIT_IS_CONFIGURED(MICROKIT_CONFIG_I3C_USE_I3C1)
void I3C1_ER_IRQHandler(void) {
   HAL_I3C_ER_IRQHandler(&DEVICE_I3C1->mcu);
}
#endif

#if MICROKIT_IS_CONFIGURED(MICROKIT_CONFIG_I3C_USE_I3C2)
void I3C2_ER_IRQHandler(void) {
   HAL_I3C_ER_IRQHandler(&DEVICE_I3C2->mcu);
}
#endif

#if MICROKIT_IS_CONFIGURED(MICROKIT_CONFIG_I3C_USE_I3C3)
void I3C3_ER_IRQHandler(void) {
   HAL_I3C_ER_IRQHandler(&DEVICE_I3C3->mcu);
}
#endif
