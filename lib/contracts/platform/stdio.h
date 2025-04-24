/** - - - - - -- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
 * @file stdio.h
 * @author D. Prause
 * @brief Standard IO interface.
 * @version 0.1
 * @date 2025-03-26
 * @copyright Copyright (c) 2025
 * - - - - - -- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#ifndef MKIT_PLATFORM_STDIO_H
#define MKIT_PLATFORM_STDIO_H

/**
 * @brief Gets one character (byte) from the system I/O.
 * @example char c = mkit_platform_stdio_get();
 * @return The character read from the device, or -1 if no character is
 * available.
 */
int mkit_platform_stdio_get(void);

/**
 * @brief Puts one character (byte) to the system I/O.
 * @example mkit_platform_stdio_put('a');
 * @param c The character to write.
 */
void mkit_platform_stdio_put(int c);

#endif