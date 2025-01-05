#include <inttypes.h>

#ifndef _SS_CAN_DEF_H_
#define _SS_CAN_DEF_H_

struct can_cntr {
    volatile uint32_t CAN_MCR, CAN_MSR, CAN_TSR, CAN_RF0R, CAN_RF1R, CAN_IER, CAN_ESR, CAN_BTR;
};

#define CAN1 ((struct can_cntr*) 0x40006400)
#define CAN2 ((struct can_cntr*) 0x40006800);

union CAN_MCR {
    struct CAN_MCR_FIELDS {
        volatile uint8_t inrq : 1;
        volatile uint8_t sleep : 1;
        volatile uint8_t txfp : 1;
        volatile uint8_t rflm : 1;
        volatile uint8_t nart : 1;
        volatile uint8_t awum : 1;
        volatile uint8_t abom : 1;
        volatile uint8_t ttcm : 1;
        volatile uint16_t reserved_1 : 7;
        volatile uint8_t reset : 1;
        volatile uint8_t dbf : 1;
        volatile uint16_t reserved_2 : 15;
    }fields;
    volatile uint32_t can_mcr;
};

union CAN_MSR {
    struct CAN_MSR_FIELDS {
        volatile uint8_t inak : 1;
        volatile uint8_t slak : 1;
        volatile uint8_t erri : 1;
        volatile uint8_t wkui : 1;
        volatile uint8_t slaki : 1;
        volatile uint8_t reserved_1 : 3;
        volatile uint8_t txm : 1;
        volatile uint8_t rxm : 1;
        volatile uint8_t samp : 1;
        volatile uint8_t rx : 1;
        volatile uint32_t reserved_2 : 20;
    } fields;
    volatile uint32_t can_msr;
};

union CAN_BTR {
    struct CAN_BTR_FIELDS {
        volatile uint16_t brp : 10;
        volatile uint8_t reserved_1 : 6;
        volatile uint8_t ts1 : 4;
        volatile uint8_t ts2 : 3;
        volatile uint8_t reserved_2 : 1;
        volatile uint8_t sjw : 2;
        volatile uint8_t reserved_4 : 4;
        volatile uint8_t lbkm  : 1;
        volatile uint8_t silm : 1;
    } fields;
    volatile uint32_t can_btr;
};



union CAN_ESR {
    struct CAN_ESR_FIELDS {
        volatile uint8_t ewgf : 1;
        volatile uint8_t epvf : 1;
        volatile uint8_t boff : 1;
        volatile uint8_t reserved_1 : 1;
        volatile uint8_t lec : 3;
        volatile uint16_t reserved_2 : 9;
        volatile uint8_t tec : 8;
        volatile uint8_t rec : 8;
    } fields;
    volatile uint32_t can_esr;
};


#endif