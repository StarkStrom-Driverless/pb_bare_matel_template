#include <inttypes.h>

#ifndef _SS_CAN_DEF_H_
#define _SS_CAN_DEF_H_

struct can_cntr {
    volatile uint32_t CAN_MCR, CAN_MSR, CAN_TSR, CAN_RF0R, CAN_RF1R, CAN_IER, CAN_ESR, CAN_BTR;
    volatile uint32_t space_1[88];
    volatile uint32_t CAN_TI0R, CAN_TDT0R, CAN_TDL0R, CAN_TDH0R, CAN_TI1R, CAN_TDT1R, CAN_TDL1R, CAN_TDH1R, CAN_TI2R, CAN_TDT2R, CAN_TDL2R, CAN_TDH2R, CAN_RI0R, CAN_RDT0R, CAN_RDL0R, CAN_RDH0R, CAN_RI1R, CAN_RDT1R, CAN_RDL1R, CAN_RDH1R;
    volatile uint32_t space_2[12];
    volatile uint32_t CAN_FMR, CAN_FM1R;
    volatile uint32_t space_3;
    volatile uint32_t CAN_FS1R;
    volatile uint32_t space_4;
    volatile uint32_t CAN_FFA1R;
    volatile uint32_t space_5;
    volatile uint32_t CAN_FA1R;
    volatile uint32_t space_6[8];
    volatile uint32_t CAN_FnRx[28][2]; //[n][x]
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

union CAN_TSR {
    struct CAN_TSR_FIELDS {
        volatile uint8_t rqcp0 : 1;
        volatile uint8_t txok0 : 1;
        volatile uint8_t alst0 : 1;
        volatile uint8_t terr0 : 1;
        volatile uint8_t reserved_1 : 3;
        volatile uint8_t abrq0 : 1;
        volatile uint8_t rqcp1 : 1;
        volatile uint8_t tx0k1 : 1;
        volatile uint8_t alst1 : 1;
        volatile uint8_t terr1 : 1;
        volatile uint8_t reserved_2 : 3;
        volatile uint8_t abrq1 : 1;
        volatile uint8_t rqcp2 : 1;
        volatile uint8_t txok2 : 1;
        volatile uint8_t alst2 : 1;
        volatile uint8_t terr2 : 1;
        volatile uint8_t reserved_3 : 3;
        volatile uint8_t abrq2 : 1;
        volatile uint8_t code : 2;
        volatile uint8_t tme : 3;
        volatile uint8_t low : 3;
    }fields;
    volatile uint32_t can_tsr;
};

union CAN_TI0R {
    struct CAN_TI0R_FIELDS {
        volatile uint8_t txrq : 1;
        volatile uint8_t rtr : 1;
        volatile uint8_t ide : 1;
        volatile uint32_t exid : 18;
        volatile uint16_t stid : 16;
    }fields;
    volatile uint32_t can_ti0r;
};



#endif