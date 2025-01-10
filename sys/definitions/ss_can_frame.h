#include <stdint.h>


#ifndef _SS_CAN_FRAME_H_
#define _SS_CAN_FRAME_H_

struct CanFrame {
    uint32_t id;
    uint8_t flags; //rtr, eff, err
    uint8_t dlc;
    uint8_t data[8];

};

#endif