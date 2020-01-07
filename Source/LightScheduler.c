/*!
 * @file
 * @brief Light scheduler implementation.
 */

#include "LightScheduler.h"
#include "uassert.h"

void LightScheduler_Init(LightScheduler_t *instance, I_DigitalOutputGroup_t *lights, I_TimeSource_t *timeSource)
{
   uassert(instance);
   uassert(lights);
   uassert(timeSource);
   instance->timeSource = timeSource;
   instance->lights = lights;
}

void LightScheduler_AddSchedule(LightScheduler_t *instance, uint8_t lightId, bool lightState, TimeSourceTickCount_t time)
{
   uassert(instance);
   uint8_t sizeSchedules = sizeof(instance->schedules) / sizeof(instance->schedules[0]);
   uint8_t i;
   for(i = 0; i < sizeSchedules; i++)
   {
      if(instance->schedules[i].active == false)
      {
         instance->schedules[i].lightId = lightId;
         instance->schedules[i].lightState = lightState;
         instance->schedules[i].time = time;
         instance->schedules[i].active = true;
         instance->numSchedules++;
         return;
      }
   }
}

void LightScheduler_Run(LightScheduler_t *instance)
{
   uassert(instance);
   TimeSourceTickCount_t time = TimeSource_GetTicks(instance->timeSource);
   uint8_t i;

   for(i = 0; i < instance->numSchedules; i++)
   {
      if((time == instance->schedules[i].time) && (instance->schedules[i].active))
      {
         DigitalOutputGroup_Write(instance->lights, instance->schedules[i].lightId, instance->schedules[i].lightState);
      }
   }
}

void LightScheduler_RemoveSchedule(LightScheduler_t *instance, uint8_t lightId, bool lightState, TimeSourceTickCount_t time)
{
   uint8_t i;
   uassert(instance);
   for(i = 0; i < instance->numSchedules; i++)
   {
      if((instance->schedules[i].lightId == lightId) && (instance->schedules[i].lightState == lightState) && (instance->schedules[i].time == time))
      {
         instance->schedules[i].active = false;
         return;
      }
   }
   uassert(false);
}