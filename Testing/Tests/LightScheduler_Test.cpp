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

   void LightSchedulerIsInitialized()
   {
      LightScheduler_Init(&scheduler, (I_DigitalOutputGroup_t *)&fakeDigitalOutputGroup, (I_TimeSource_t *)&fakeTimeSource);
   }

   void NothingShouldHappen()
   {
   }

   void EventScheduledAt(LightScheduler_t * instance, DigitalOutputChannel_t lightId, bool lightState, TimeSourceTickCount_t time)
   {
      LightScheduler_AddSchedule(instance, lightId, lightState, time);
   }

   void RemoveScheduleAt(LightScheduler_t * instance, DigitalOutputChannel_t lightId, bool lightState, TimeSourceTickCount_t time)
   {
      LightScheduler_RemoveSchedule(instance, lightId, lightState, time);
   }

   void WhenTimeIs(TimeSourceTickCount_t time)
   {
      mock().expectOneCall("GetTicks").onObject(&fakeTimeSource.interface).andReturnValue(time);
   }

   void LightShouldBeOn(DigitalOutputChannel_t lightId)
   {
      mock().expectOneCall("Write").onObject(&fakeDigitalOutputGroup.interface).withParameter("channel", lightId).withParameter("state", true);
   }

   void LightShouldBeOff(DigitalOutputChannel_t lightId)
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
}

TEST(LightScheduler, ShouldScheduleOneLightOn)
{
   LightSchedulerIsInitialized();
   EventScheduledAt(&scheduler, 1, true, 10);
   WhenTimeIs(10);
   LightShouldBeOn(1);
   WhenSchedulerIsRun(&scheduler);
}

TEST(LightScheduler, ShouldScheduleTwoLightsOn)
{
   LightSchedulerIsInitialized();
   EventScheduledAt(&scheduler, 1, true, 10);
   EventScheduledAt(&scheduler, 2, true, 10);
   WhenTimeIs(10);
   LightShouldBeOn(1);
   LightShouldBeOn(2);
   WhenSchedulerIsRun(&scheduler);
}

TEST(LightScheduler, ShouldScheduleTwoLightsOnAtDifferentTimes)
{
   LightSchedulerIsInitialized();
   EventScheduledAt(&scheduler, 1, true, 10);
   EventScheduledAt(&scheduler, 2, true, 11);
   WhenTimeIs(10);
   LightShouldBeOn(1);
   WhenSchedulerIsRun(&scheduler);
   WhenTimeIs(11);
   LightShouldBeOn(2);
   WhenSchedulerIsRun(&scheduler);
}

TEST(LightScheduler, ShouldScheduleOneLightOnThenOff)
{
   LightSchedulerIsInitialized();
   EventScheduledAt(&scheduler, 1, true, 9);
   EventScheduledAt(&scheduler, 1, false, 10);
   WhenTimeIs(9);
   LightShouldBeOn(1);
   WhenSchedulerIsRun(&scheduler);
   WhenTimeIs(10);
   LightShouldBeOff(1);
   WhenSchedulerIsRun(&scheduler);
}

TEST(LightScheduler, ShouldDoNothingIfAtDifferentTime)
{
   LightSchedulerIsInitialized();
   EventScheduledAt(&scheduler, 1, true, 10);
   WhenTimeIs(11);
   NothingShouldHappen();
   WhenSchedulerIsRun(&scheduler);
}

TEST(LightScheduler, ShouldDoNothingAfterRemoveScheduleOfOneLight)
{
   LightSchedulerIsInitialized();
   EventScheduledAt(&scheduler, 1, true, 9);
   RemoveScheduleAt(&scheduler, 1, true, 9);
   WhenTimeIs(9);
   NothingShouldHappen();
   WhenSchedulerIsRun(&scheduler);
}

TEST(LightScheduler, ShouldDoNothingOnOneMoreThanMaxSchedules)
{
   LightSchedulerIsInitialized();
   EventScheduledAt(&scheduler, 1, true, 10);
   EventScheduledAt(&scheduler, 2, true, 10);
   EventScheduledAt(&scheduler, 3, true, 10);
   EventScheduledAt(&scheduler, 4, true, 10);
   EventScheduledAt(&scheduler, 5, true, 10);
   EventScheduledAt(&scheduler, 6, true, 10);
   EventScheduledAt(&scheduler, 7, true, 10);
   EventScheduledAt(&scheduler, 8, true, 10);
   EventScheduledAt(&scheduler, 9, true, 10);
   EventScheduledAt(&scheduler, 10, true, 10);
   EventScheduledAt(&scheduler, 11, true, 10);
   WhenTimeIs(10);
   LightShouldBeOn(1);
   LightShouldBeOn(2);
   LightShouldBeOn(3);
   LightShouldBeOn(4);
   LightShouldBeOn(5);
   LightShouldBeOn(6);
   LightShouldBeOn(7);
   LightShouldBeOn(8);
   LightShouldBeOn(9);
   LightShouldBeOn(10);
   WhenSchedulerIsRun(&scheduler);
}

TEST(LightScheduler, ShouldAddScheduleInInactiveScheduleSpot)
{
   LightSchedulerIsInitialized();
   EventScheduledAt(&scheduler, 1, true, 10);
   EventScheduledAt(&scheduler, 2, true, 10);
   EventScheduledAt(&scheduler, 3, true, 10);
   EventScheduledAt(&scheduler, 4, true, 10);
   EventScheduledAt(&scheduler, 5, true, 10);
   EventScheduledAt(&scheduler, 6, true, 10);
   EventScheduledAt(&scheduler, 7, true, 10);
   EventScheduledAt(&scheduler, 8, true, 10);
   EventScheduledAt(&scheduler, 9, true, 10);
   EventScheduledAt(&scheduler, 10, true, 10);
   RemoveScheduleAt(&scheduler, 5, true, 10);
   EventScheduledAt(&scheduler, 11, true, 10);
   WhenTimeIs(10);
   LightShouldBeOn(1);
   LightShouldBeOn(2);
   LightShouldBeOn(3);
   LightShouldBeOn(4);
   LightShouldBeOn(6);
   LightShouldBeOn(7);
   LightShouldBeOn(8);
   LightShouldBeOn(9);
   LightShouldBeOn(10);
   LightShouldBeOn(11);
   WhenSchedulerIsRun(&scheduler);
}

// TEST(LightScheduler, RemoveInvalidSchedule)
// {
//    LightSchedulerIsInitialized();
//    EventScheduledAt(&scheduler, 1, true, 20);
//    CHECK_ASSERTION_FAILED(RemoveScheduleAt(&scheduler, 2, false, 24));
// }
