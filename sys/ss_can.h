#ifndef _SS_CAN_H_
#define _SS_CAN_H_

#include <stdint.h>
#include "pin_declarations.h"
#include "ss_gpio.h"
#include "ss_makros.h"
#include "ss_can_def.h"
#include "ss_rcc_def.h"
#include "systick_handles.h"
#include "ss_systick.h"
#include "ss_nvic.h"
#include "ss_can_receive_fifo.h"

extern Systick_Handle handle2;

struct Fifo can_receive_fifos[2];






struct PinConfig* get_can_pins(uint8_t can_id) {
    struct PinConfig* pin_config = 0;
    switch (can_id)
    {
        case 1: pin_config = &pin_config_can1; break;
        case 2: pin_config = &pin_config_can2; break;
        default: break;
    }
    return pin_config;
}


struct can_cntr* get_can_ptr(uint8_t can_id) {
    struct can_cntr* can_ptr = 0;
    switch (can_id)
    {
        case 1: can_ptr = CAN1; break;
        case 2: can_ptr = CAN2; break;
        default: break;
    }
    return can_ptr;
}

int8_t CAN_Init(uint8_t can_id, uint32_t baud) {
    struct PinConfig *pin_config = get_can_pins(can_id);
    struct can_cntr* can = get_can_ptr(can_id);
    if(gpio_set_pin_configs(pin_config) == -1) return -1;
    gpio_write(pin_config->pin_config[2].pin, GPIO_ON);

    (void)baud;


    GPIO(PINBANK(pin_config->pin_config[0].pin))->OSPEEDR = (uint32_t)((1 << PINNO(pin_config->pin_config[0].pin)) | (1 << PINNO(pin_config->pin_config[0].pin)));
    GPIO(PINBANK(pin_config->pin_config[1].pin))->OSPEEDR = (uint32_t)((1 << PINNO(pin_config->pin_config[1].pin)) | (1 << PINNO(pin_config->pin_config[1].pin)));


    union CAN_MCR* can_mcr = ((union CAN_MCR*)(&can->CAN_MCR));
    union CAN_MSR* can_msr = ((union CAN_MSR*)(&can->CAN_MSR));
    
    
    can_mcr->fields.inrq = 1;
    while(can_msr->fields.inak == 0);
    
    can_mcr->fields.sleep = 0;
    while(can_msr->fields.slak == 1);

    can_mcr->fields.abom = 1;

    uint8_t filter_id = (CAN1 == can) ? 0 : 14;
    CAN1->CAN_FMR |= (1 << 0);
    CAN1->CAN_FA1R &= ~(uint32_t)(1 << filter_id);
    CAN1->CAN_FS1R |= (1 << filter_id);
    CAN1->CAN_FM1R &= ~(uint32_t)(1 << filter_id);
    CAN1->CAN_FFA1R &= ~(uint32_t)(1 << filter_id);
    CAN1->CAN_FnRx[filter_id][0] = 0x00000000;
    CAN1->CAN_FnRx[filter_id][1] = 0x00000000;
    CAN1->CAN_FA1R |= (1 << filter_id);
    CAN1->CAN_FMR &= ~(uint32_t)(1 << 0);

    // Das geht
    can->CAN_BTR |= 0;

    can->CAN_BTR &= (uint32_t)~(0xF << 16);
    can->CAN_BTR |= (9 << 16);

    can->CAN_BTR &= (uint32_t)~(0x7 << 20);
    can->CAN_BTR |= (4 << 20);

    can_mcr->fields.inrq = 0;
    while(can_msr->fields.inak == 1);

    //Für später - Muss warscheinlich PRIO in NVIC gepackt noch werden
    can->CAN_IER |= (1 << 1);

    if (can == CAN1) {
        enable_nvic_interrupt(20);
    } else {
        enable_nvic_interrupt(64);
    }
    return 0;
}

void can_send(struct CanFrame* can_frame, struct can_cntr* can){

    while( ((union CAN_TSR*)(&can->CAN_TSR))->fields.tme == 0 ) {
            if (handle_timer(&handle2)) {
                gpio_write(pin_error, GPIO_TOGGLE);
            }
    }

    can->CAN_TI0R = (can_frame->id << 21);
    can->CAN_TDT0R = can_frame->dlc;
    can->CAN_TDL0R = (uint32_t)((can_frame->data[3] <<  24) | (can_frame->data[2] <<  16) | (can_frame->data[1] <<  8) | (can_frame->data[0]));
    can->CAN_TDH0R = (uint32_t)((can_frame->data[7] <<  24) | (can_frame->data[6] <<  16) | (can_frame->data[5] <<  8) | (can_frame->data[4]));

    ((union CAN_TI0R*)(&can->CAN_TI0R))->fields.txrq = 1;

}

void CAN1_RX0_IRQHandler(void) {
    if (CAN1->CAN_RF0R & (0x3 << 0)) {
        struct CanFrame can_frame;
        can_frame.id = (CAN1->CAN_RI0R >> 21) & 0x7FF;
        can_frame.flags = 0;
        can_frame.dlc =  CAN1->CAN_RDT0R & 0xF;
        for(uint8_t i = 0; i < 8; i++) {
            can_frame.data[i] = (uint8_t) ((i < 4) ? (CAN1->CAN_RDL0R >> i * 8) : (CAN1->CAN_RDH0R >> ((i - 4)*8) ));
        }        
        fifo_add_can_frame(&can_receive_fifos[0], &can_frame);
        gpio_write(pin_heartbeat, GPIO_TOGGLE);
        CAN1->CAN_RF0R |= (1 << 5);
    }
}

void CAN2_RX0_IRQHandler(void) {
    if (CAN2->CAN_RF0R & (0x3 << 0)) {
        struct CanFrame can_frame;
        can_frame.id = (CAN2->CAN_RI0R >> 21) & 0x7FF;
        can_frame.flags = 0;
        can_frame.dlc =  CAN2->CAN_RDT0R & 0xF;
        for(uint8_t i = 0; i < 8; i++) {
            can_frame.data[i] = (uint8_t) ((i < 4) ? (CAN2->CAN_RDL0R >> i * 8) : (CAN2->CAN_RDH0R >> ((i - 4)*8) ));
        }        
        fifo_add_can_frame(&can_receive_fifos[1], &can_frame);
        gpio_write(pin_heartbeat, GPIO_TOGGLE);
        CAN2->CAN_RF0R |= (1 << 5);
    }
}



#endif 