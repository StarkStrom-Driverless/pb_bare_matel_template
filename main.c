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

  //uint16_t pa2 = PIN('A', 2);
  //gpio_set_mode(pa2, GPIO_MODE_AF);
  //gpio_init_pwm(pa2, 159, 2000);

  SPI_Init(1, 1000000);

  if(CAN_Init(1, 1000000) == -1) ErrorHandler();
  if(CAN_Init(2, 1000000) == -1) ErrorHandler();



  systick_init(16000000/1000);

  if (uart_init(1, 115200) == -1) ErrorHandler();

  //gpio_write(pa2, 1000);

  gpio_write(pin_heartbeat, GPIO_OFF);
  

  for (;;) {


    if (handle_timer(&handle1)) {
      
      if(!is_fifo_empty(&can_receive_fifos[0])) {
        struct CanFrame can_frame;
        fifo_remove_can_frame(&can_receive_fifos[0], &can_frame);
        can_send(&can_frame, 1);
        gpio_write(pin_blue_one, GPIO_TOGGLE);
      }


      uart_write_buf(1, "TestHallo \n\r", 12);

      gpio_write(pin_heartbeat, GPIO_TOGGLE);
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


