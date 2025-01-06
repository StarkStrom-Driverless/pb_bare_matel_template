#include <stdint.h>
#include "ss_nvic.def.h"

#ifndef _SS_NVIC_H_
#define _SS_NVIC_H_

void enable_nvic_interrupt(int isr_number) {
    NVIC_ISR->iser[isr_number / 32] |= (1 << (isr_number % 32));
}

#endif