/*!
 * @file
 * @brief Digital output group mock.
 */

#ifndef DIGITALOUTPUTGROUP_MOCK_H
#define DIGITALOUTPUTGROUP_MOCK_H

#include "I_DigitalOutputGroup.h"

typedef struct
{
   I_DigitalOutputGroup_t interface;
} DigitalOutputGroup_Mock_t;

void DigitalOutputGroup_Mock_Init(DigitalOutputGroup_Mock_t *instance);

#endif
