/*!

* @file

* @brief Tests for light scheduler implementation.

*/

extern "C"
{
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

   void LightSchedulerIsInitialized()
   {
      LightScheduler_Init(&scheduler, (I_DigitalOutputGroup_t *)&fakeDigitalOutputGroup, (I_TimeSource_t *)&fakeTimeSource);
   }

   void EventScheduledAt(LightScheduler_t *instance, DigitalOutputChannel_t lightId, bool lightState, TimeSourceTickCount_t time)
   {
      LightScheduler_AddSchedule(instance, lightId, lightState, time);  
   }

   void WhenTimeIs(TimeSourceTickCount_t time)
   {
      mock().expectOneCall("GetTicks").onObject(&fakeTimeSource.interface).andReturnValue(time);
   }

   void WhichLightIsOn(DigitalOutputChannel_t lightId)
   {
      mock().expectOneCall("Write").onObject(&fakeDigitalOutputGroup.interface).withParameter("channel", lightId).withParameter("state", true);
   }

   void WhichLightIsOff(DigitalOutputChannel_t lightId)
   {
      mock().expectOneCall("Write").onObject(&fakeDigitalOutputGroup.interface).withParameter("channel", lightId).withParameter("state", false);
   }

   void WhenSchedulerIsRun(LightScheduler_t *instance)
   {
      LightScheduler_Run(instance);
   }
};

TEST(LightScheduler, ShouldScheduleOneLightOn)
{
   LightSchedulerIsInitialized();
   EventScheduledAt(&scheduler, 1, true, 10);
   WhenTimeIs(10);
   WhichLightIsOn(1);
   WhenSchedulerIsRun(&scheduler);
}

TEST(LightScheduler, ShouldScheduleTwoLightsOn)
{
   LightSchedulerIsInitialized();
   EventScheduledAt(&scheduler, 1, true, 10);
   EventScheduledAt(&scheduler, 2, true, 10);
   WhenTimeIs(10);
   WhichLightIsOn(1);
   WhichLightIsOn(2);
   WhenSchedulerIsRun(&scheduler);
}

TEST(LightScheduler, ShouldScheduleTwoLightsOnAtDifferentTimes)
{
   LightSchedulerIsInitialized();
   EventScheduledAt(&scheduler, 1, true, 10);
   EventScheduledAt(&scheduler, 2, true, 11);
   WhenTimeIs(10);
   WhichLightIsOn(1);
   WhenSchedulerIsRun(&scheduler);
   WhenTimeIs(11);
   WhichLightIsOn(2);
   WhenSchedulerIsRun(&scheduler);
}

TEST(LightScheduler, ShouldScheduleOneLightOnThenOff)
{
   LightSchedulerIsInitialized();
   EventScheduledAt(&scheduler, 1, true, 9);
   EventScheduledAt(&scheduler, 1, false, 10);
   WhenTimeIs(9);
   WhichLightIsOn(1);
   WhenSchedulerIsRun(&scheduler);
   WhenTimeIs(10);
   WhichLightIsOff(1);
   WhenSchedulerIsRun(&scheduler);
}