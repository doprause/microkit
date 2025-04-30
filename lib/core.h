
#ifndef MICROKIT_CORE_H
#define MICROKIT_CORE_H

/**
 * Configuration macros:
 * Usage:
 * #if MICROKIT_IS_CONFIGURED(MICROKIT_CONFIG_USE_XYZ)
 *    Code for feature XYZ
 * #endif
 *
 */
#define MICROKIT_IS_CONFIGURED(x) x == 1

typedef enum {
   MKIT_MODULE_STATE_UNINITIALIZED,
   MKIT_MODULE_STATE_STOPPED,
   MKIT_MODULE_STATE_RUNNING
} ModuleState;

#endif // MICROKIT_CORE_H