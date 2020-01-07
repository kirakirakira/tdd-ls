/*!

* @file

* @brief Tests for light scheduler implementation.

*/

extern "C"
{
#include <string.h>
#include "LightScheduler.h"
}
#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"
#include "DigitalOutputGroup_Mock.h"
#include "TimeSource_Mock.h"
#include "uassert_test.h"

TEST_GROUP(LightScheduler)
{
   LightScheduler_t scheduler;
   DigitalOutputGroup_Mock_t fakeDigitalOutputGroup;
   TimeSource_Mock_t fakeTimeSource;

   void setup()
   {
      DigitalOutputGroup_Mock_Init(&fakeDigitalOutputGroup);
      TimeSource_Mock_Init(&fakeTimeSource);
   }

   void WhenLightSchedulerIsInitialized()
   {
      LightScheduler_Init(&scheduler, (I_DigitalOutputGroup_t *)&fakeDigitalOutputGroup, (I_TimeSource_t *)&fakeTimeSource);
   }

   void NothingShouldHappen()
   {
   }

   void WhenEventScheduledAt(LightScheduler_t * instance, DigitalOutputChannel_t lightId, bool lightState, TimeSourceTickCount_t time)
   {
      LightScheduler_AddSchedule(instance, lightId, lightState, time);
   }

   void WhenLightScheduledOnAt(LightScheduler_t * instance, DigitalOutputChannel_t lightId, TimeSourceTickCount_t time)
   {
      LightScheduler_AddSchedule(instance, lightId, true, time);
   }

   void WhenLightScheduledOffAt(LightScheduler_t * instance, DigitalOutputChannel_t lightId, TimeSourceTickCount_t time)
   {
      LightScheduler_AddSchedule(instance, lightId, false, time);
   }

   void AfterScheduleMaximumSchedulesOnAt(LightScheduler_t * instance, TimeSourceTickCount_t time)
   {
      uint8_t i;
      for(i = 0; i < MAX_SCHEDULES; i++)
      {
         WhenLightScheduledOnAt(&scheduler, i + 1, time);
      }
   }

   void ThenLights1to10ShouldBeOn(LightScheduler_t * instance)
   {
      uint8_t i;
      for(i = 0; i < MAX_SCHEDULES; i++)
      {
         ThenLightShouldBeOn(i + 1);
      }
   }

   void AfterRemoveScheduleAt(LightScheduler_t * instance, DigitalOutputChannel_t lightId, bool lightState, TimeSourceTickCount_t time)
   {
      LightScheduler_RemoveSchedule(instance, lightId, lightState, time);
   }

   void WhenTimeIs(TimeSourceTickCount_t time)
   {
      mock().expectOneCall("GetTicks").onObject(&fakeTimeSource.interface).andReturnValue(time);
   }

   void ThenLightShouldBeOn(DigitalOutputChannel_t lightId)
   {
      mock().expectOneCall("Write").onObject(&fakeDigitalOutputGroup.interface).withParameter("channel", lightId).withParameter("state", true);
   }

   void ThenLightShouldBeOff(DigitalOutputChannel_t lightId)
   {
      mock().expectOneCall("Write").onObject(&fakeDigitalOutputGroup.interface).withParameter("channel", lightId).withParameter("state", false);
   }

   void WhenSchedulerIsRun(LightScheduler_t * instance)
   {
      LightScheduler_Run(instance);
   }
};

TEST(LightScheduler, InitNullChecks)
{
   CHECK_ASSERTION_FAILED(LightScheduler_Init(NULL, (I_DigitalOutputGroup_t *)&fakeDigitalOutputGroup, (I_TimeSource_t *)&fakeTimeSource));
   CHECK_ASSERTION_FAILED(LightScheduler_Init(&scheduler, NULL, (I_TimeSource_t *)&fakeTimeSource));
   CHECK_ASSERTION_FAILED(LightScheduler_Init(&scheduler, (I_DigitalOutputGroup_t *)&fakeDigitalOutputGroup, NULL));
}

TEST(LightScheduler, CheckNullSchedulerAddEventFails)
{
   CHECK_ASSERTION_FAILED(WhenEventScheduledAt(NULL, 1, true, 10));
}

TEST(LightScheduler, CheckNullSchedulerRemoveEventFails)
{
   CHECK_ASSERTION_FAILED(AfterRemoveScheduleAt(NULL, 1, true, 10));
}

TEST(LightScheduler, CheckNullSchedulerRunFails)
{
   CHECK_ASSERTION_FAILED(WhenSchedulerIsRun(NULL));
}

TEST(LightScheduler, ShouldScheduleOneLightOn)
{
   WhenLightSchedulerIsInitialized();
   WhenLightScheduledOnAt(&scheduler, 1, 10);
   WhenTimeIs(10);
   ThenLightShouldBeOn(1);
   WhenSchedulerIsRun(&scheduler);
}

TEST(LightScheduler, ShouldScheduleTwoLightsOn)
{
   WhenLightSchedulerIsInitialized();
   WhenLightScheduledOnAt(&scheduler, 1, 10);
   WhenLightScheduledOnAt(&scheduler, 2, 10);
   WhenTimeIs(10);
   ThenLightShouldBeOn(1);
   ThenLightShouldBeOn(2);
   WhenSchedulerIsRun(&scheduler);
}

TEST(LightScheduler, ShouldScheduleTwoLightsOnAtDifferentTimes)
{
   WhenLightSchedulerIsInitialized();
   WhenLightScheduledOnAt(&scheduler, 1, 10);
   WhenLightScheduledOnAt(&scheduler, 2, 11);
   WhenTimeIs(10);
   ThenLightShouldBeOn(1);
   WhenSchedulerIsRun(&scheduler);
   WhenTimeIs(11);
   ThenLightShouldBeOn(2);
   WhenSchedulerIsRun(&scheduler);
}

TEST(LightScheduler, ShouldScheduleOneLightOnThenOff)
{
   WhenLightSchedulerIsInitialized();
   WhenLightScheduledOnAt(&scheduler, 1, 9);
   WhenLightScheduledOffAt(&scheduler, 1, 10);
   WhenTimeIs(9);
   ThenLightShouldBeOn(1);
   WhenSchedulerIsRun(&scheduler);
   WhenTimeIs(10);
   ThenLightShouldBeOff(1);
   WhenSchedulerIsRun(&scheduler);
}

TEST(LightScheduler, ShouldDoNothingIfAtDifferentTime)
{
   WhenLightSchedulerIsInitialized();
   WhenLightScheduledOnAt(&scheduler, 1, 10);
   WhenTimeIs(11);
   NothingShouldHappen();
   WhenSchedulerIsRun(&scheduler);
}

TEST(LightScheduler, ShouldDoNothingAfterRemoveScheduleOfOneLight)
{
   WhenLightSchedulerIsInitialized();
   WhenLightScheduledOnAt(&scheduler, 1, 9);
   AfterRemoveScheduleAt(&scheduler, 1, true, 9);
   WhenTimeIs(9);
   NothingShouldHappen();
   WhenSchedulerIsRun(&scheduler);
}

TEST(LightScheduler, ShouldDoNothingOnOneMoreThanMaxSchedules)
{
   WhenLightSchedulerIsInitialized();
   AfterScheduleMaximumSchedulesOnAt(&scheduler, 10);
   WhenLightScheduledOnAt(&scheduler, 11, 10);
   WhenTimeIs(10);
   ThenLights1to10ShouldBeOn(&scheduler);
   WhenSchedulerIsRun(&scheduler);
}

TEST(LightScheduler, ShouldAddScheduleInInactiveScheduleSpot)
{
   WhenLightSchedulerIsInitialized();
   AfterScheduleMaximumSchedulesOnAt(&scheduler, 10);
   AfterRemoveScheduleAt(&scheduler, 5, true, 10);
   WhenLightScheduledOnAt(&scheduler, 11, 10);
   WhenTimeIs(10);
   ThenLightShouldBeOn(1);
   ThenLightShouldBeOn(2);
   ThenLightShouldBeOn(3);
   ThenLightShouldBeOn(4);
   ThenLightShouldBeOn(6);
   ThenLightShouldBeOn(7);
   ThenLightShouldBeOn(8);
   ThenLightShouldBeOn(9);
   ThenLightShouldBeOn(10);
   ThenLightShouldBeOn(11);
   WhenSchedulerIsRun(&scheduler);
}

TEST(LightScheduler, RemoveInvalidSchedule)
{
   WhenLightSchedulerIsInitialized();
   WhenLightScheduledOnAt(&scheduler, 1, 20);
   CHECK_ASSERTION_FAILED(AfterRemoveScheduleAt(&scheduler, 2, false, 24));
}