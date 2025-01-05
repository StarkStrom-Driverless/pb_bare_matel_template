// Copyright (c) 2022 Cesanta Software Limited
// All rights reserved

#include <inttypes.h>
#include <stdbool.h>

//SYS
#include "ss_makros.h"
#include "ss_gpio.h"
#include "ss_delay.h"
#include "ss_systick.h"
#include "ss_timer.h"
#include "ss_can.h"

// USR_CONFIGS
#include "systick_handles.h"
#include "systick_isr.h"
#include "pin_declarations.h"

extern Systick_Handle handle1;
extern Systick_Handle handle2;

extern uint16_t pin_heartbeat;
extern uint16_t pin_error;
extern uint16_t pin_blue_one;
extern uint16_t pin_blue_two;

extern uint16_t can1_tx;
extern uint16_t can1_rx;



int main(void) {




  inti_pb_pins();

  CAN_Init(can1_tx, can1_rx);

  systick_init(16000000/1000);


  uint8_t data[8] = {1, 2, 3, 4, 5, 6, 7};

  for (;;) {
    if (handle_timer(&handle1)) {
      CAN_Send(0x01, data, 8);
      gpio_write(pin_blue_one, GPIO_TOGGLE);
    }
    
  }
  return 0;
}


