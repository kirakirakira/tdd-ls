/*!
 * @file
 * @brief Digital Output group consisting of multiple Digital Outputs organized into channels.
 */

#ifndef I_DIGITALOUTPUTGROUP_H
#define I_DIGITALOUTPUTGROUP_H

#include <stdint.h>
#include <stdbool.h>
#include "uassert.h"

typedef uint16_t DigitalOutputChannel_t;

struct I_DigitalOutputGroup_Api_t;

/*!
 * Generic Digital Output group.
 */
typedef struct
{
   /*!
    * API for interacting with a particular instance of a Digital Output group.
    */
   const struct I_DigitalOutputGroup_Api_t *api;
} I_DigitalOutputGroup_t;

/*!
 * Interface for interacting with a GPIO group.  API should be accessed using wrapper calls below.
 */
typedef struct I_DigitalOutputGroup_Api_t
{
   void (*Write)(I_DigitalOutputGroup_t *instance, const DigitalOutputChannel_t channel, const bool state);
} I_DigitalOutputGroup_Api_t;

/*!
 * Write to a digital output channel.
 * @pre instance != NULL
 * @param instance The digital output group.
 * @param channel The digital output channel.
 * @param state The state to write.
 */
#define DigitalOutputGroup_Write(instance, channel, state) \
   (instance)->api->Write((instance), (channel), (state))

#endif
