/** - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
 * @file systick.h
 * @author D. Prause
 * @brief Systick platform API.
 * @version 0.1
 * @date 2025-05-13
 * @copyright Copyright (c) 2025
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#ifndef MICROKIT_PLATFORM_SYSTICK_H
#define MICROKIT_PLATFORM_SYSTICK_H

#include "libs/microkit/lib/types.h"

/**
 * @brief Signature for tick handler functions.
 * @param tickValue The current value of the systick counter.
 * @param target The target object for the tick handler.
 */
typedef void (*SystickHandler)(UInt16 tickValue, void* target);

/**
 * @brief Abstract pointer to the Systick instance.
 */
typedef struct SystickObject* SystickInstance;

/**
 * @brief The Systick interface provides access to the system tick
 * counter that generates interrupts at a fixed rate. Other objects can
 * attach tick handlers to the systick.
 */
typedef struct {

   /**
    * @brief Initializes the systick.
    * @note This function is called before any other function is called.
    */
   void (*init)(void);

   /**
    * @brief Starts the systick.
    */
   void (*start)(void);

   /**
    * @brief Stops the systick.
    */
   void (*stop)(void);

   /**
    * @brief Gets the tick interval.
    * @returns The tick interval in milliseconds.
    */
   UInt16 (*interval)(void);

   /**
    * @brief Gets the current value of the systick counter.
    * The counter is a 32-bit rollover up counter which is incremented
    * on each system tick event.
    * @returns The current value of the systick counter.
    */
   UInt16 (*tickValue)(void);

   /**
    * @brief Adds a tick handler that is called on each tick event.
    * @param handler The tick handler function.
    * @param target The target object for the handler.
    */
   void (*addTickHandler)(const SystickHandler handler, void* target);

   /**
    * @brief Removes the tick handler.
    * @param handler The tick handler function.
    */
   void (*removeTickHandler)(const SystickHandler handler);

} SystickInterface;

extern const SystickInterface Systick;

#endif // MICROKIT_PLATFORM_SYSTICK_H
