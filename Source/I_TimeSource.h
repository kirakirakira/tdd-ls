/*!
 * @file
 * @brief Generic time source.
 */

#ifndef I_TIMESOURCE_H
#define I_TIMESOURCE_H

#include <stdint.h>

/*!
 * Tick count.
 */
typedef uint16_t TimeSourceTickCount_t;

/*!
 * Time source object.
 */
typedef struct
{
   /*!
    * Time source API used to interact with the time source object.
    */
   const struct I_TimeSource_Api_t *api;
} I_TimeSource_t;

/*!
 * Interface for interacting with a timer.  API should be accessed using wrapper calls below.
 */
typedef struct I_TimeSource_Api_t
{
   /*!
    * Get the current tick count from a time source.
    * @pre instance != NULL
    * @param instance The time source.
    * @return The current tick count.
    */
   TimeSourceTickCount_t (*GetTicks)(I_TimeSource_t *instance);
} I_TimeSource_Api_t;

#define TimeSource_GetTicks(instance) \
   (instance)->api->GetTicks((instance))

#endif
