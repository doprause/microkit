/**
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
 *                 oo                            dP       oo   dP
 *                                              88            88
 *      88d8b.d8b. dP .d8888b. 88d888b. .d8888b. 88  .dP  dP d8888P
 *      88'`88'`88 88 88'  `"" 88'  `88 88'  `88 88888"   88   88
 *      88  88  88 88 88.  ... 88       88.  .88 88  `8b. 88   88
 *      dP  dP  dP dP `88888P' dP       `88888P' dP   `YP dP   dP
 *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
 * @file termcolor.h
 * @author Dominik Prause (dominik@inspirent.io)
 * @version 0.1
 * @date 2025-04-17
 * @copyright Copyright (c) 2025 - All rights reserved.
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
 */
#ifndef MICROKIT_TERMCOLOR_H
#define MICROKIT_TERMCOLOR_H

#define TERMCOLOR_RESET "\033[00m"
#define TERMCOLOR_BOLD "\033[1m"
#define TERMCOLOR_DARK "\033[2m"
#define TERMCOLOR_UNDERLINE "\033[4m"
#define TERMCOLOR_BLINK "\033[5m"
#define TERMCOLOR_REVERSE "\033[7m"
#define TERMCOLOR_CONCEALED "\033[8m"
#define TERMCOLOR_GRAY "\033[90m"
#define TERMCOLOR_GREY "\033[90m"
#define TERMCOLOR_RED "\033[31m"
#define TERMCOLOR_GREEN "\033[32m"
#define TERMCOLOR_YELLOW "\033[33m"
#define TERMCOLOR_BLUE "\033[34m"
#define TERMCOLOR_MAGENTA "\033[35m"
#define TERMCOLOR_CYAN "\033[36m"
#define TERMCOLOR_WHITE "\033[37m"
#define TERMCOLOR_BG_GRAY "\033[40m"
#define TERMCOLOR_BG_GREY "\033[40m"
#define TERMCOLOR_BG_RED "\033[41m"
#define TERMCOLOR_BG_GREEN "\033[42m"
#define TERMCOLOR_BG_YELLOW "\033[43m"
#define TERMCOLOR_BG_BLUE "\033[44m"
#define TERMCOLOR_BG_MAGENTA "\033[45m"
#define TERMCOLOR_BG_CYAN "\033[46m"
#define TERMCOLOR_BG_WHITE "\033[47m"

/**
 * @brief colorizes the given text with the given ANSI color
 */
#define colorize(text, color) color text TERMCOLOR_RESET

/**
 * @brief colorizes the given text with the given ANSI
 * foreground and background color
 */
#define colorize2(text, foreground, background) \
   foreground background text TERMCOLOR_RESET

#endif