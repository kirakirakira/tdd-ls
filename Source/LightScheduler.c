/*!
 * @file
 * @brief Light scheduler implementation.
 */

#include "LightScheduler.h"

void LightScheduler_Init(LightScheduler_t *instance, I_DigitalOutputGroup_t *lights, I_TimeSource_t *timeSource)
{
    instance->timeSource = timeSource;
    instance->lights = lights;
}

void LightScheduler_AddSchedule(LightScheduler_t *instance, uint8_t lightId, bool lightState, TimeSourceTickCount_t time)
{
    instance->schedules[instance->numSchedules].lightId = lightId;
    instance->schedules[instance->numSchedules].lightState = lightState;
    instance->schedules[instance->numSchedules].time = time;
    instance->numSchedules++;
}

void LightScheduler_Run(LightScheduler_t *instance)
{
    TimeSourceTickCount_t time = TimeSource_GetTicks(instance->timeSource);
    uint8_t i;

    for(i = 0; i < instance->numSchedules; i++)
    {
        if(time == instance->schedules[i].time) {
            DigitalOutputGroup_Write(instance->lights, instance->schedules[i].lightId, instance->schedules[i].lightState); 
        }
    }
}
