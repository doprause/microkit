#include "libs/microkit/lib/assert.h"
#include "libs/microkit/lib/core.h"
#include "libs/microkit/lib/debug.h"
#include "libs/microkit/lib/microkit.h"
#include "libs/microkit/lib/platform/drivers/i3c.h"
#include "libs/microkit/lib/utils/math.h"
#include "microkit/config/i3c.h"

#include "stm32h5xx_hal.h"

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
   MKIT_I3C_BUS_STATE_READING,
   MKIT_I3C_BUS_STATE_WRITING,
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
   //    I2cStatus status;
   //    I2cSlaveState slave;
   I3C_HandleTypeDef mcu;
};

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
   👉 Configuration
   - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
// #if MICROKIT_IS_CONFIGURED(MICROKIT_CONFIG_I2C_USE_I2C1)
// struct I2cDeviceObject DEVICE_I2C1_INSTANCE = {
//     .busState = MKIT_I3C_BUS_STATE_RESET,
//     .state = MKIT_DRIVER_STATE_UNINITIALIZED,
//     .targetCount = 0,
//     .targetDescriptorsCount = 0,
//     .mcu = {0},
// };
// MicrokitI2cDevice DEVICE_I2C1 = &DEVICE_I2C1_INSTANCE;
// #endif // MICROKIT_CONFIG_I2C_USE_I2C1

#if MICROKIT_IS_CONFIGURED(MICROKIT_CONFIG_I3C_USE_I3C2)
struct I3cDeviceObject DEVICE_I3C2_INSTANCE = {
    .busState = MKIT_I3C_BUS_STATE_RESET,
    .state = MKIT_DRIVER_STATE_UNINITIALIZED,
    .targetCount = 0,
    .targetDescriptorsCount = 0,
    .mcu = {0},
};
MicrokitI3cDevice DEVICE_I3C2 = &DEVICE_I3C2_INSTANCE;
#endif // MICROKIT_CONFIG_I3C_USE_I3C2

// #if MICROKIT_IS_CONFIGURED(MICROKIT_CONFIG_I2C_USE_I2C3)
// struct I2cDeviceObject DEVICE_I2C3_INSTANCE = {
//     .busState = MKIT_I3C_BUS_STATE_RESET,
//     .state = MKIT_DRIVER_STATE_UNINITIALIZED,
//     .targetCount = 0,
//     .targetDescriptorsCount = 0,
//     .mcu = {0},
// };
// MicrokitI2cDevice DEVICE_I2C3 = &DEVICE_I2C3_INSTANCE;
// #endif // MICROKIT_CONFIG_I2C_USE_I2C3

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
   👉 Private functions
   - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
// static inline void rearm_i2c_slave_mode(const MicrokitI2cDevice device) {

//    device->status.isRepeatedStart = false;

//    HAL_I2C_EnableListen_IT(&device->mcu);
// }

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
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
void private_start_dynamic_address_assignment(MicrokitI3cDevice device) {

   device->busState = MKIT_I3C_BUS_STATE_DYN_ADDR_ASSIGNMENT_RUNNING;

   if (HAL_I3C_Ctrl_DynAddrAssign_IT(&device->mcu, I3C_ONLY_ENTDAA) != HAL_OK) {
      device->busState = MKIT_I3C_BUS_STATE_ERROR;
   }
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
   DEVICE_I3C2->mcu.Init.CtrlBusCharacteristic.SCLPPLowDuration = 0x0f;
   DEVICE_I3C2->mcu.Init.CtrlBusCharacteristic.SCLI3CHighDuration = 0x09;
   DEVICE_I3C2->mcu.Init.CtrlBusCharacteristic.SCLODLowDuration = 0x0f;
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

   private_start_dynamic_address_assignment(device);
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
#endif

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

// /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
// StatusOrNumber microkit_i2c_transmit(
//     const MicrokitI2cDevice device, const UInt8 deviceAddress,
//     UInt8* data, Size dataSize, Bool async) {

//    ASSERT_NOT_NULL_POINTER(device);
//    ASSERT(device->state == MKIT_DRIVER_STATE_RUNNING);

//    if (HAL_I2C_GetState(&device->mcu) != HAL_I2C_STATE_READY) {
//       return STATUS_ERROR;
//    }

//    if (device->status.masterTransmitComplete == false) {
//       return STATUS_ERROR;
//    }

//    HAL_Delay(10); // Without this delay, the transfer is not reliable

//    device->status.masterTransmitComplete = false;

//    HAL_StatusTypeDef status = HAL_I2C_Master_Transmit_IT(
//        &device->mcu, deviceAddress << 1, data, dataSize);

//    // If we are not in async mode, we need to wait for the transfer to complete
//    if (!async) {
//       while (!device->status.masterTransmitComplete)
//          ;
//    }

//    return status == HAL_OK ? (int)dataSize : STATUS_ERROR;
// }

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

// /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
// StatusOrNumber microkit_i2c_memory_write(
//     const MicrokitI2cDevice device, const UInt8 deviceAddress,
//     const UInt16 memoryAddress, const UInt16 memoryAddressSize,
//     const UInt8* data, const Size dataSize, const Bool async) {

//    ASSERT_NOT_NULL_POINTER(device);
//    ASSERT(device->state == MKIT_DRIVER_STATE_RUNNING);

//    if (device->status.memoryWriteComplete == false) {
//       return STATUS_ERROR;
//    }

//    HAL_Delay(10); // Without this delay, the transfer is not reliable

//    device->status.memoryWriteComplete = false;

//    HAL_StatusTypeDef status =
//        HAL_I2C_Mem_Write_IT(
//            &device->mcu,
//            (UInt16)deviceAddress << 1,
//            memoryAddress,
//            memoryAddressSize,
//            (UInt8*)data,
//            dataSize);

//    // If we are not in async mode, we need to wait for the transfer to complete
//    if (!async) {
//       while (!device->status.memoryWriteComplete) {
//          // Wait for the transfer to complete
//       };
//    }

//    return status == HAL_OK ? (int)dataSize : STATUS_ERROR;
// }

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
   👉 Vendor specific code
   - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
void HAL_I3C_TgtReqDynamicAddrCallback(I3C_HandleTypeDef* handle, uint64_t targetPayload) {

   // Called when a target responded to a dynamic address request

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

   // Called when all targets have been assigned a dynamic address

   MicrokitI3cDevice device = private_get_device_from_handle(handle);

   device->busState = MKIT_I3C_BUS_STATE_DYN_ADDR_ASSIGNMENT_FINISHED;
}

// void HAL_I2C_MasterRxCpltCallback(I2C_HandleTypeDef* handle) {

//    // I2C master receive completed

//    MicrokitI2cDevice device = private_get_device_from_handle(handle);

//    ASSERT_NOT_NULL_POINTER(device);

//    if (device->callbacks.masterReceiveCompleteHandler) {
//       device->callbacks.masterReceiveCompleteHandler(device);
//    }

//    device->status.masterReceiveComplete = true;
// }

// /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
// void HAL_I2C_MasterTxCpltCallback(I2C_HandleTypeDef* handle) {

//    // I2C master transmit completed

//    MicrokitI2cDevice device = private_get_device_from_handle(handle);

//    ASSERT_NOT_NULL_POINTER(device);

//    if (device->callbacks.masterTransmitCompleteHandler) {
//       device->callbacks.masterTransmitCompleteHandler(device);
//    }

//    device->status.masterTransmitComplete = true;
// }

// /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
// void HAL_I2C_MemRxCpltCallback(I2C_HandleTypeDef* handle) {

//    // I2C memory read completed

//    MicrokitI2cDevice device = private_get_device_from_handle(handle);

//    ASSERT_NOT_NULL_POINTER(device);

//    if (device->callbacks.memoryReadCompleteHandler) {
//       device->callbacks.memoryReadCompleteHandler(device);
//    }

//    device->status.memoryReadComplete = true;
// }

// /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
// void HAL_I2C_MemTxCpltCallback(I2C_HandleTypeDef* handle) {

//    // I2C memory write completed

//    MicrokitI2cDevice device = private_get_device_from_handle(handle);

//    ASSERT_NOT_NULL_POINTER(device);

//    if (device->callbacks.memoryWriteCompleteHandler) {
//       device->callbacks.memoryWriteCompleteHandler(device);
//    }

//    device->status.memoryWriteComplete = true;
// }

// /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
// #if MICROKIT_IS_CONFIGURED(CONFIG_I2C_USE_SLAVE_MODE) // 👉 Slave mode code
// /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
// void HAL_I2C_AddrCallback(I2C_HandleTypeDef* handle, uint8_t TransferDirection,
//                           uint16_t AddrMatchCode) {
//    // DEBUG_PRINT("I2C ADDRESS MATCH\n");

//    // I2C START detected / Transmission has started

//    MicrokitI2cDevice device = private_get_device_from_handle(handle);

//    ASSERT_NOT_NULL_POINTER(device->callbacks.slaveTransmissionStartedHandler);
//    ASSERT_NOT_NULL_POINTER(device->callbacks.slaveTransmissionErrorHandler);

//    if (TransferDirection == I2C_DIRECTION_TRANSMIT) {
//       // DEBUG_PRINT("I2C TX\n");

//       // Master wants to transmit data. So we - the slave - receive it

//       device->callbacks.slaveTransmissionStartedHandler(
//           device, MKIT_I2C_DIRECTION_WRITE, device->status.isRepeatedStart);

//       // I2C_FIRST_FRAME means SLAVE receives the first byte of a sequence.
//       if (HAL_I2C_Slave_Seq_Receive_IT(handle, &device->slave.rxByte, 1,
//                                        I2C_FIRST_FRAME) != HAL_OK) {
//          //     device->callbacks.slaveByteReceivedHandler(device, &byte);
//          // }
//          // else {
//          device->callbacks.slaveTransmissionErrorHandler(device,
//                                                          MKIT_I2C_ERROR);
//       }
//    } else {
//       // DEBUG_PRINT("I2C RX\n");

//       // Master wants to receive data. So we - the slave - transmit it

//       device->callbacks.slaveTransmissionStartedHandler(
//           device, MKIT_I2C_DIRECTION_READ, device->status.isRepeatedStart);

//       device->callbacks.slaveByteRequestedHandler(device,
//                                                   &device->slave.txByte);

//       // I2C_FIRST_FRAME means SLAVE transmits the first byte of a sequence.
//       if (HAL_I2C_Slave_Seq_Transmit_IT(handle, &device->slave.txByte, 1,
//                                         I2C_FIRST_FRAME) != HAL_OK) {
//          device->callbacks.slaveTransmissionErrorHandler(device,
//                                                          MKIT_I2C_ERROR);
//       }
//    }

//    // The isRepeatedStart is set to true on first START condition so it is true
//    // on consecutive START conditions. It will be reset to false on STOP
//    // condition.
//    device->status.isRepeatedStart = true;
// }

// /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
// void HAL_I2C_SlaveTxCpltCallback(I2C_HandleTypeDef* handle) {
//    // DEBUG_PRINT("I2C SLAVE TX COMPLETE\n");

//    MicrokitI2cDevice device = private_get_device_from_handle(handle);

//    // static uint8 byte = 0x00;
//    device->callbacks.slaveByteRequestedHandler(device, &device->slave.txByte);
//    // DEBUG_PRINT("I2C SLAVE TX COMPLETE > 0x%02x\n", device->slave.txByte);

//    // I2C_NEXT_FRAME means SLAVE is ready to transmit another byte.
//    HAL_I2C_Slave_Seq_Transmit_IT(handle, &device->slave.txByte, 1,
//                                  I2C_NEXT_FRAME);
// }

// /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
// void HAL_I2C_SlaveRxCpltCallback(I2C_HandleTypeDef* handle) {
//    // DEBUG_PRINT("I2C SLAVE RX COMPLETE\n");

//    MicrokitI2cDevice device = private_get_device_from_handle(handle);

//    // static uint8 byte = 0x00;
//    if (HAL_I2C_Slave_Seq_Receive_IT(handle, &device->slave.rxByte, 1,
//                                     I2C_NEXT_FRAME) == HAL_OK) {
//       device->callbacks.slaveByteReceivedHandler(device, &device->slave.rxByte);
//    } else {
//       device->callbacks.slaveTransmissionErrorHandler(device, MKIT_I2C_ERROR);
//    }

//    // TODO: Check if we need to handle the LAST_FRAME

//    // if (rxcount < RX_BUFFER_SIZE) {
//    // 	if (rxcount == RX_BUFFER_SIZE-1)
//    // 	{
//    // 		HAL_I2C_Slave_Seq_Receive_IT(handle, &byte, 1, I2C_LAST_FRAME);
//    // 	}
//    // 	else
//    // 	{
//    // 		HAL_I2C_Slave_Seq_Receive_IT(handle, &byte, 1, I2C_NEXT_FRAME);
//    // 	}
//    // }

//    // if (rxcount == RX_BUFFER_SIZE)
//    // {
//    // 	// Invoke processing callback
//    // }
// }

// /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
// void HAL_I2C_ListenCpltCallback(I2C_HandleTypeDef* handle) {
//    // DEBUG_PRINT("I2C LISTEN COMPLETE\n");

//    // I2C STOP detected / Transmission is complete

//    MicrokitI2cDevice device = private_get_device_from_handle(handle);

//    device->callbacks.slaveTransmissionStoppedHandler(device);

//    rearm_i2c_slave_mode(device);
// }

// /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
// void HAL_I2C_ErrorCallback(I2C_HandleTypeDef* handle) {
//    // DEBUG_PRINT("I2C ERROR\n");

//    MicrokitI2cDevice device = private_get_device_from_handle(handle);

//    // TODO: Implement error statistics

//    UInt32 errorcode = HAL_I2C_GetError(handle);

//    if (errorcode == HAL_I2C_ERROR_BERR) {
//       // BERR error

//       // DEBUG_PRINT("I2C BUS ERROR\n");

//       HAL_I2C_DeInit(handle);
//       HAL_I2C_Init(handle);
//    } else if (errorcode == HAL_I2C_ERROR_AF) {
//       // AF error

//       // DEBUG_PRINT("I2C ACK ERROR\n");

//       // ACK Failure is raised when master sends a STOP before buffer is full.
//       // Instead an error, we treat it as a regular STOP condition.
//       // We don't have to call the slaveTransmissionStoppedHandler(device)
//       // callback because the listen complete handler is called which then calls
//       // the slaveTransmissionStoppedHandler(device) callback
//    } else {
//       Microkit.logger.log(MKIT_LOG_LEVEL_WARN, "UNHANDLED I2C ERROR [%d]", errorcode);
//    }

//    if (device->mode == MKIT_I2C_MODE_MASTER) {
//       if (device->callbacks.masterReceiveCompleteHandler) {
//          device->callbacks.masterReceiveCompleteHandler(device);
//       }

//       if (device->callbacks.masterTransmitCompleteHandler) {
//          device->callbacks.masterTransmitCompleteHandler(device);
//       }

//       if (device->callbacks.memoryReadCompleteHandler) {
//          device->callbacks.memoryReadCompleteHandler(device);
//       }

//       if (device->callbacks.memoryWriteCompleteHandler) {
//          device->callbacks.memoryWriteCompleteHandler(device);
//       }

//       device->status.masterReceiveComplete = true;
//       device->status.masterTransmitComplete = true;
//       device->status.memoryReadComplete = true;
//       device->status.memoryWriteComplete = true;
//    }

//    if (device->mode == MKIT_I2C_MODE_SLAVE) {
//       rearm_i2c_slave_mode(device);
//    }
// }
// /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
// #endif // CONFIG_I2C_USE_SLAVE_MODE
// /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

// /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
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

// /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
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
