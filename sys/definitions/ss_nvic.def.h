
#include <stdint.h>

#ifndef _SS_NVIC_DEF_H_
#define _SS_NVIC_DEF_H_

struct nvic_isr {
    volatile uint32_t iser[8];    //0x100 - 0x11C
    volatile uint32_t space_1[24];      //0x120 - 0x17C
    volatile uint32_t icer[8];         //0x180 - 0x19C
    volatile uint32_t space_2[24];      //0x1A0 - 0x1FC
    volatile uint32_t ispr[8];         //0x200 - 0x21C
    volatile uint32_t space_3[24];      //0x220 - 0x27C
    volatile uint32_t icpr[8];          //0x280 - 0x29C
};

#define NVIC_ISR ((struct nvic_isr*)(0xE000E100))



#endif