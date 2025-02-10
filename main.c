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
#include "ss_spi.h"
#include "ss_uart.h"

// USR_CONFIGS
#include "systick_handles.h"
#include "systick_isr.h"
#include "pin_declarations.h"


#include "ads131m04.h"

extern Systick_Handle handle1;
extern Systick_Handle handle2;

extern uint16_t pin_heartbeat;
extern uint16_t pin_error;
extern uint16_t pin_blue_one;
extern uint16_t pin_blue_two;


extern struct Fifo can_receive_fifos[2];

void ErrorHandler(void);

static inline void spin(volatile uint32_t count) {
  while (count--) asm("nop");
}



int main(void) {

  inti_pb_pins();



  if (init_ads131() == -1) ErrorHandler();


  if(CAN_Init(1, 1000000) == -1) ErrorHandler();




  systick_init(16000000/1000);

  gpio_write(pin_heartbeat, GPIO_OFF);

  struct adcOutput adc_vals;
  struct CanFrame can_frame_adc;

  can_frame_adc.id = 0x10;
  can_frame_adc.dlc = 4;

  for (;;) {


    if (handle_timer(&handle1)) {
      
      if(!is_fifo_empty(&can_receive_fifos[0])) {
        struct CanFrame can_frame;
        fifo_remove_can_frame(&can_receive_fifos[0], &can_frame);
        can_send(&can_frame, 1);
        gpio_write(pin_blue_one, GPIO_TOGGLE);
      }

      gpio_write(pin_heartbeat, GPIO_TOGGLE);
    }

    if (ads_is_data_ready()) {
      ads_read(&adc_vals);
      can_frame_adc.data[0] = (uint8_t)(adc_vals.ch[1]);
      can_frame_adc.data[1] = (uint8_t)(adc_vals.ch[1] >> 8);
      can_frame_adc.data[2] = (uint8_t)(adc_vals.ch[1] >> 16);
      can_frame_adc.data[3] = (uint8_t)(adc_vals.ch[1] >> 24);
      can_send(&can_frame_adc, 1);

    }


  }
  return 0;
}

void ErrorHandler(void) {
  for(;;) {
    gpio_write(pin_error, GPIO_TOGGLE);
    spin(1000000);
  }
}

void DefaultHandler(void) {
    gpio_write(pin_heartbeat, GPIO_TOGGLE);
}


