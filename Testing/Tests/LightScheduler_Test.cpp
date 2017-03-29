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

//      LightScheduler_Init(&scheduler, (I_DigitalOutputGroup_t *)&fakeDigitalOutputGroup, (I_TimeSource_t *)&fakeTimeSource);
   }

   void teardown()
   {
   }
};

TEST(LightScheduler, FirstTest)
{
   FAIL("Oh good the tests are working");
}
