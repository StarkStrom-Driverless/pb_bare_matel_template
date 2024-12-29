#include <inttypes.h>

#ifndef _SS_DELAY_H_
#define _SS_DELAY_H_


static inline void delay(volatile uint32_t count) {
  while (count--) asm("nop");
}

#endif