#ifndef _SYSTICK_ISR_H_
#define _SYSTICK_ISR_H_

#include "systick_handles.h"

extern Systick_Handle handle1;
extern Systick_Handle handle2;

void SysTick_Handler(void) {
  handle1.tick++;
  handle2.tick++;
}



#endif