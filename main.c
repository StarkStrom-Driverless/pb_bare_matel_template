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





void uint32ToUint8Array_LE(uint32_t value, uint8_t array[4]) {
    array[3] = (uint8_t)(value & 0xFF);
    array[2] = (uint8_t)((value >> 8) & 0xFF);
    array[1] = (uint8_t)((value >> 16) & 0xFF);
    array[0] = (uint8_t)((value >> 24) & 0xFF);
}





int main(void) {

  inti_pb_pins();

  init_ads131_wrapper();

  adcStartup();

  if(CAN_Init(1, 1000000) == -1) ErrorHandler();




  systick_init(16000000/1000);

  gpio_write(pin_heartbeat, GPIO_OFF);

  
  adc_channel_data adc_data;

  struct CanFrame adc_can_frame;
  adc_can_frame.id = 0xFF;
  adc_can_frame.dlc = 4;
  



  for (;;) {

    /*
    if (handle_timer(&handle1)) {
      
      
      if(!is_fifo_empty(&can_receive_fifos[0])) {
        struct CanFrame can_frame;
        fifo_remove_can_frame(&can_receive_fifos[0], &can_frame);
        can_send(&can_frame, 1);
        gpio_write(pin_blue_one, GPIO_TOGGLE);
      }

      gpio_write(pin_heartbeat, GPIO_TOGGLE);
    }  
    */
    
    if (handle_timer(&handle1)) {
      uint32ToUint8Array_LE(adc_data.channel3, adc_can_frame.data);
      can_send(&adc_can_frame, 1);
    }


    if (data_received()) {
      readData(&adc_data);
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


