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

// USR_CONFIGS
#include "systick_handles.h"
#include "systick_isr.h"

extern Systick_Handle handle1;




int main(void) {
  uint16_t led = PIN('A', 2);
  RCC->AHB1ENR |= BIT(PINBANK(led));
  gpio_set_mode(led, GPIO_MODE_AF);
  gpio_init_pwm(led, 159, 2000);

  systick_init(16000000/1000);

  uint16_t pwm_val = 0;

  for (;;) {
    if (handle_timer(&handle1)) {
      gpio_write(led, pwm_val);
      pwm_val += 1;
      if (pwm_val > 2000) pwm_val = 0;
    }
  }
  return 0;
}

