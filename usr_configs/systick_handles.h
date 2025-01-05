#ifndef _SYSTICK_HANDLES_H_
#define _SYSTICK_HANDLES_H_

#include "ss_systick.h"

Systick_Handle handle1 = {.timer = 0, .period=1000, .tick = 0};
Systick_Handle handle2 = {.timer = 0, .period=1000, .tick = 0};

#endif