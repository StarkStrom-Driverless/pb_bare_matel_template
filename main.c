// Copyright (c) 2022 Cesanta Software Limited
// All rights reserved

#include <inttypes.h>
#include <stdbool.h>

//SYS
#include "ss_makros.h"
#include "ss_gpio.h"
#include "ss_delay.h"
#include "ss_systick.h"

// USR_CONFIGS
#include "systick_handles.h"

extern Systick_Handle handle1;

void SysTick_Handler(void) {
  handle1.tick++;
}






int main(void) {
  uint16_t led = PIN('C', 1);
  RCC->AHB1ENR |= BIT(PINBANK(led));
  gpio_set_mode(led, GPIO_MODE_OUTPUT);

  systick_init(16000000/1000);

  for (;;) {
    if (handle_timer(&handle1)) {
      gpio_write(led, GPIO_TOGGLE, 0);
    }

  }
  return 0;
}

