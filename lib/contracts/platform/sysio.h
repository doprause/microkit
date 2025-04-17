/** - - - - - -- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
 * @file sysio.h
 * @author D. Prause
 * @brief SysIO VAL interface.
 * @version 0.1
 * @date 2025-03-26
 * @copyright Copyright (c) 2025
 * - - - - - -- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#ifndef MICROKIT_PLATFORM_SYSIO_H
#define MICROKIT_PLATFORM_SYSIO_H

/**
 * @brief Gets one character (byte) from the system I/O.
 * @example char c = sysio_get();
 * @return The character read from the device, or -1 if no character is
 * available.
 */
int sysio_get(void);

/**
 * @brief Puts one character (byte) to the system I/O.
 * @example sysio_put('a');
 * @param c The character to write.
 */
void sysio_put(int c);

#endif