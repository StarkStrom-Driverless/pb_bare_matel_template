#include <inttypes.h>
#include <stdbool.h>
#include "ss_makros.h"
#include "ss_rcc_def.h"

#ifndef _SS_TIMER_DEF_H_
#define _SS_TIMER_DEF_H_

struct tim {
    volatile uint32_t CR1, CR2, SMCR, DIER, SR, EGR, CCMR1, CCMR2, CCER, CNT, PSC, ARR, RCR, CCR1, CCR2, CCR3, CCR4, BDTR, DCR, DMAR;
};

#define TIM1 ((struct tim *) 0x40010000)
#define TIM8 ((struct tim *) 0x40010400)
#define TIM2 ((struct tim *) 0x40000000)
#define TIM5 ((struct tim *) 0x40000C00)
#define TIM3 ((struct tim *) 0x40000400)
#define TIM12 ((struct tim *) 0x40001800)

union PRC {
    struct FIELDS_PRC {
        volatile uint16_t psc;
        volatile uint16_t reserved;
    }fields;
    volatile uint32_t psc;
};

union ARR {
    struct FIELDS_RR {
        volatile uint16_t arr;
        volatile uint16_t reserved;
    } fields;
    volatile uint32_t arr;
};

union CCMR {
    struct FIELDS_CCMR {
        volatile uint8_t cc1 : 2;
        volatile uint8_t oc1fe : 1;
        volatile uint8_t oc1pe : 1;
        volatile uint8_t oc1m : 3;
        volatile uint8_t oc1ce : 1;
        volatile uint8_t cc2s : 2;
        volatile uint8_t oc2fe : 1;
        volatile uint8_t oc2pe : 1;
        volatile uint8_t oc2m : 3;
        volatile uint8_t oc2ce : 1;
        volatile uint16_t reserved : 16;
    } fields;
    volatile uint32_t ccmr;
};

union CCER {
    struct FIELDS_CCER {
        volatile uint8_t cc1e : 1;
        volatile uint8_t cc1p : 1;
        volatile uint8_t cc1ne : 1;
        volatile uint8_t cc1np : 1;
        volatile uint8_t cc2e : 1;
        volatile uint8_t cc2p : 1;
        volatile uint8_t cc2ne : 1;
        volatile uint8_t cc2np : 1;
        volatile uint8_t cc3e : 1;
        volatile uint8_t cc3p : 1;
        volatile uint8_t cc3ne : 1;
        volatile uint8_t cc3np : 1;
        volatile uint8_t cc4e : 1;
        volatile uint8_t cc4p : 1;
        volatile uint8_t reserved_1 : 1;
        volatile uint8_t cc4np : 1;
        volatile uint16_t reserved_2 : 16;
    } fields;
    volatile uint32_t ccer;
};

union CR1 {
    struct FIELDS_CR1 {
        volatile uint8_t cen : 1;
        volatile uint8_t udis : 1;
        volatile uint8_t urs : 1;
        volatile uint8_t opm : 1;
        volatile uint8_t dir : 1;
        volatile uint8_t cms : 2;
        volatile uint8_t arpe : 1;
        volatile uint8_t ckd : 2;
        volatile uint32_t reserved : 22; 
    } fields;
    volatile uint32_t cr1;
};

union BDTR
{
    struct FIELDS_BDTR {
        volatile uint8_t dt : 8;
        volatile uint8_t lock : 2;
        volatile uint8_t ossi : 1;
        volatile uint8_t ossr : 1;
        volatile uint8_t bke : 1;
        volatile uint8_t bkp : 1;
        volatile uint8_t aoe : 1;
        volatile uint8_t moe : 1;
    } fields;
    volatile uint32_t bdtr;
};


struct pin_timer_config {
    struct tim* timer;
    uint8_t channel;
    uint8_t enable;
    volatile uint32_t* rcc_ptr;
    uint8_t rcc_bit_pos;
    uint8_t af : 4;
    uint8_t advanced;
};

struct pin_timer_config pin_timer_configs[48] = {
    [PIN('A', 0)]  = {TIM5, 1, 0, &RCC->APB1ENR, 3, 0x02, 0},
    [PIN('A', 1)]  = {TIM5, 2, 0, &RCC->APB1ENR, 3, 0x02, 0}, 
    [PIN('A', 2)]  = {TIM5, 3, 0, &RCC->APB1ENR, 3, 0x02, 0},
    [PIN('A', 3)]  = {TIM5, 4, 0, &RCC->APB1ENR, 3, 0x02, 0},
    [PIN('A', 4)]  = {0,0, 0, 0, 0, 0, 0},
    [PIN('A', 5)]  = {TIM2, 1, 0, &RCC->APB1ENR, 0, 0x01, 0},
    [PIN('A', 6)]  = {TIM3, 1, 0, &RCC->APB1ENR, 1, 0x02, 0},
    [PIN('A', 7)]  = {TIM3, 2, 0, &RCC->APB1ENR, 1, 0x02, 0},
    [PIN('A', 8)]  = {TIM1, 1, 0, &RCC->APB2ENR, 0, 0x01, 1},
    [PIN('A', 9)]  = {TIM1, 2, 0, &RCC->APB2ENR, 0, 0x01, 1},
    [PIN('A', 10)] = {TIM1, 3, 0, &RCC->APB2ENR, 0, 0x01, 1},
    [PIN('A', 11)] = {TIM1, 4, 0, &RCC->APB2ENR, 0, 0x01, 1},
    [PIN('A', 12)] = {0,0, 0, 0, 0, 0, 0},
    [PIN('A', 13)] = {0,0, 0, 0, 0, 0, 0},
    [PIN('A', 14)] = {0,0, 0, 0, 0, 0, 0},
    [PIN('A', 15)] = {TIM2, 1, 0, &RCC->APB1ENR, 0, 0x01, 0},
    [PIN('B', 0)]  = {TIM3, 3, 0, &RCC->APB1ENR, 1, 0x02, 0},
    [PIN('B', 1)]  = {TIM3, 4, 0, &RCC->APB1ENR, 1, 0x02, 0},
    [PIN('B', 2)]  = {0,0, 0, 0, 0, 0, 0},
    [PIN('B', 3)]  = {0,0, 0, 0, 0, 0, 0},
    [PIN('B', 4)]  = {0,0, 0, 0, 0, 0, 0},
    [PIN('B', 5)]  = {0,0, 0, 0, 0, 0, 0},
    [PIN('B', 6)]  = {0,0, 0, 0, 0, 0, 0},
    [PIN('B', 7)]  = {0,0, 0, 0, 0, 0, 0},
    [PIN('B', 8)]  = {0,0, 0, 0, 0, 0, 0},
    [PIN('B', 9)]  = {0,0, 0, 0, 0, 0, 0},
    [PIN('B', 10)] = {TIM2, 3, 0, &RCC->APB1ENR, 0, 0x01, 0},
    [PIN('B', 11)] = {TIM2, 4, 0, &RCC->APB1ENR, 0, 0x01, 0},
    [PIN('B', 12)] = {0,0, 0, 0, 0, 0, 0},
    [PIN('B', 13)] = {0,0, 0, 0, 0, 0, 0},
    [PIN('B', 14)] = {TIM12, 1, 0, &RCC->APB1ENR, 6, 0x09, 0},
    [PIN('B', 15)] = {TIM12, 2, 0, &RCC->APB1ENR, 6, 0x09, 0},
    [PIN('C', 0)]  = {0,0, 0, 0, 0, 0, 0},
    [PIN('C', 1)]  = {0,0, 0, 0, 0, 0, 0},
    [PIN('C', 2)]  = {0,0, 0, 0, 0, 0, 0},
    [PIN('C', 3)]  = {0,0, 0, 0, 0, 0, 0},
    [PIN('C', 4)]  = {0,0, 0, 0, 0, 0, 0},
    [PIN('C', 5)]  = {0,0, 0, 0, 0, 0, 0},
    [PIN('C', 6)]  = {TIM8, 1, 0, &RCC->APB2ENR, 1, 0x03, 1},
    [PIN('C', 7)]  = {TIM8, 2, 0, &RCC->APB2ENR, 1, 0x03, 1},
    [PIN('C', 8)]  = {TIM8, 3, 0, &RCC->APB2ENR, 1, 0x03, 1},
    [PIN('C', 9)]  = {TIM8, 4, 0, &RCC->APB2ENR, 1, 0x03, 1},
    [PIN('C', 10)] = {0,0, 0, 0, 0, 0, 0},
    [PIN('C', 11)] = {0,0, 0, 0, 0, 0, 0},
    [PIN('C', 12)] = {0,0, 0, 0, 0, 0, 0},
    [PIN('C', 13)] = {0,0, 0, 0, 0, 0, 0},
    [PIN('C', 14)] = {0,0, 0, 0, 0, 0, 0},
    [PIN('C', 15)] = {0,0, 0, 0, 0, 0, 0}
};



#endif