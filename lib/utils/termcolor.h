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

#define ANSI_RESET "\033[00m"
#define ANSI_BOLD "\033[1m"
#define ANSI_DARK "\033[2m"
#define ANSI_UNDERLINE "\033[4m"
#define ANSI_BLINK "\033[5m"
#define ANSI_REVERSE "\033[7m"
#define ANSI_CONCEALED "\033[8m"
#define ANSI_GRAY "\033[30m"
#define ANSI_GREY "\033[30m"
#define ANSI_RED "\033[31m"
#define ANSI_GREEN "\033[32m"
#define ANSI_YELLOW "\033[33m"
#define ANSI_BLUE "\033[34m"
#define ANSI_MAGENTA "\033[35m"
#define ANSI_CYAN "\033[36m"
#define ANSI_WHITE "\033[37m"
#define ANSI_BG_GRAY "\033[40m"
#define ANSI_BG_GREY "\033[40m"
#define ANSI_BG_RED "\033[41m"
#define ANSI_BG_GREEN "\033[42m"
#define ANSI_BG_YELLOW "\033[43m"
#define ANSI_BG_BLUE "\033[44m"
#define ANSI_BG_MAGENTA "\033[45m"
#define ANSI_BG_CYAN "\033[46m"
#define ANSI_BG_WHITE "\033[47m"

#define colorize(text, color) color text ANSI_RESET

#endif