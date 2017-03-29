/*!
 * @file
 * @brief Simple mock of a time source object, used for testing.
 */

#ifndef TIMESOURCE_MOCK_H
#define TIMESOURCE_MOCK_H

extern "C"
{
#include "I_TimeSource.h"
}

typedef struct
{
   I_TimeSource_t interface;
} TimeSource_Mock_t;

void TimeSource_Mock_Init(TimeSource_Mock_t *instance);

#endif
