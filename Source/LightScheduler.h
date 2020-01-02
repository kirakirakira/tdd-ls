/*!
 * @file
 * @brief Simple light scheduler.  Uses a digital output group to write to the controlled lights.  Uses
 * a time source to get the current time.
 */

#ifndef LIGHTSCHEDULER_H
#define LIGHTSCHEDULER_H

#include <stdint.h>
#include <stdbool.h>

#include "I_TimeSource.h"
#include "I_DigitalOutputGroup.h"

#define MAX_SCHEDULES (10)

typedef struct
{
   TimeSourceTickCount_t time;
   DigitalOutputChannel_t lightId;
   bool lightState;
} Schedule_t;

typedef struct
{
   Schedule_t schedules[MAX_SCHEDULES];
   uint8_t numSchedules;
   I_TimeSource_t *timeSource;
   I_DigitalOutputGroup_t *lights;
} LightScheduler_t;

/*!
 * Initialize a light scheduler.
 * @param instance The light scheduler.
 * @param lights A digital output group that can be used to control the lights.  Light ID x will be
 *    channel x in the digital output group.
 * @param timeSource This is how the light scheduler will get the current time.
 */
void LightScheduler_Init(LightScheduler_t *instance, I_DigitalOutputGroup_t *lights, I_TimeSource_t *timeSource);

/*!
 * Schedule a light to be turned on/off.
 * @param instance The light scheduler.
 * @param lightId The light ID that will be controlled by the scheduler.
 * @param lightState The state that will be written for the light (on/off).
 * @param time The light will be controlled when the time from the TimeSource reaches this value.
 *    The lightState should be written to the light with lightId at this time.
 */
void LightScheduler_AddSchedule(LightScheduler_t *instance, uint8_t lightId, bool lightState, TimeSourceTickCount_t time);

/*!
 * Run a light scheduler.  The light scheduler will run all schedules that are due.
 * @param instance The light scheduler.
 */
void LightScheduler_Run(LightScheduler_t *instance);

#endif
