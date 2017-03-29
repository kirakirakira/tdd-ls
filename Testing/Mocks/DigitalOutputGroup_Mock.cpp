/*!
 * @file
 * @brief Implementation of a digital output group mock.
 */

#include "DigitalOutputGroup_Mock.h"
#include "CppUTestExt/MockSupport.h"

static void Write(I_DigitalOutputGroup_t *instance, const DigitalOutputChannel_t channel, const bool state)
{
   mock().actualCall("Write")
      .onObject(instance)
      .withParameter("channel", channel)
      .withParameter("state", state);
}

static const I_DigitalOutputGroup_Api_t api =
   { Write };

void DigitalOutputGroup_Mock_Init(DigitalOutputGroup_Mock_t *instance)
{
   instance->interface.api = &api;
}
