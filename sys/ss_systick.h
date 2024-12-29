#include <inttypes.h>
#include <stdbool.h>

#include "ss_systick_def.h"
#include "ss_makros.h"
#include "ss_rcc_def.h"

#ifndef _SS_SYSTICK_H_
#define _SS_SYSTICK_H_

typedef struct Systick_Handle
{
    uint32_t timer;
    uint32_t period;
    volatile uint32_t tick;

} Systick_Handle;


static inline void systick_init(uint32_t ticks) {
  if ((ticks - 1) > 0xffffff) return;
  SYSTICK->LOAD = ticks - 1;
  SYSTICK->VAL = 0;
  SYSTICK->CTRL = BIT(0) | BIT(1) | BIT(2);
  RCC->APB2ENR |= BIT(14);
}

bool timer_expired(uint32_t *t, uint32_t prd, uint32_t now) {
  if (now + prd < *t) *t = 0;
  if (*t == 0) *t = now + prd;
  if (*t > now) return false;
  *t = (now - *t) > prd ? now + prd : *t + prd;
  return true;
}

bool handle_timer(Systick_Handle* handle) {
  if (handle->tick + handle->period < handle->timer) handle->timer = 0;
  if (handle->timer == 0) handle->timer = handle->tick + handle->period;
  if (handle->timer > handle->tick) return false;
  handle->timer = (handle->tick - handle->timer) > handle->period ? handle->tick + handle->period : handle->timer + handle->period;
  return true;

}

#endif