#include <inttypes.h>
#include <stdbool.h>

#ifndef _SS_SYSTICK_DEF_H_
#define _SS_SYSTICK_DEF_H_

struct systick {
  volatile uint32_t CTRL, LOAD, VAL, CALIB;
};
#define SYSTICK ((struct systick *) 0xe000e010)

#endif