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
 * @file types.h
 * @author Dominik Prause (dominik@inspirent.io)
 * @version 0.1
 * @date 2025-04-17
 * @copyright Copyright (c) 2025 - All rights reserved.
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
 */
#ifndef MICROKIT_TYPES_H
#define MICROKIT_TYPES_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef bool Bool;

typedef unsigned int UInt;
typedef uint8_t UInt8;
typedef uint16_t UInt16;
typedef uint32_t UInt32;
typedef uint64_t UInt64;

typedef int Int;
typedef int8_t Int8;
typedef int16_t Int16;
typedef int32_t Int32;
typedef int64_t Int64;

typedef size_t Size;

typedef int Status;
typedef int StatusOrChar;
typedef int StatusOrNumber;

// Status codes
typedef enum {
   STATUS_OK = 0,
   STATUS_ERROR = -1,
   STATUS_EOF = -2,
   STATUS_TIMEOUT = -3,

} StatusCodes;

#define FALSE 0
#define TRUE 1

#define IS_ERROR(status) (status < STATUS_OK)
#define IS_VALUE(value) (value >= 0)

// Array helpers
#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))

// Unused variables
#ifdef UNUSED_ARG
#undef UNUSED_ARG
#warning                                                                       \
    "Undefined UNUSED_ARG and redefined it as #define UNUSED_ARG(x) (void)(x) in the global scope."
#else
#define UNUSED_ARG(x) (void)(x)
#endif

#endif
