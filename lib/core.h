
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

#endif // MICROKIT_CORE_H