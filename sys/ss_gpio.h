#include "ss_makros.h"
#include "ss_gpio_def.h"
#include "ss_rcc_def.h"
#include "ss_timer_def.h"
#include "ss_pin.h"

#ifndef _GPIO_H
#define _GPIO_H

enum {GPIO_OFF, GPIO_ON, GPIO_TOGGLE};


static inline void gpio_set_mode(uint16_t pin, uint8_t mode) {
  RCC->AHB1ENR |= BIT(PINBANK(pin));

  struct gpio *gpio = GPIO(PINBANK(pin));
  int n = PINNO(pin);
  gpio->MODER &= ~(3U << (n * 2));
  gpio->MODER |= (mode & 3U) << (n * 2);
}

static inline void gpio_set_af(uint16_t pin, uint8_t af_num) {
  struct gpio *gpio = GPIO(PINBANK(pin));  // GPIO bank
  int n = PINNO(pin);                      // Pin number
  gpio->AFR[n >> 3] &= ~(15UL << ((n & 7) * 4));
  gpio->AFR[n >> 3] |= ((uint32_t) af_num) << ((n & 7) * 4);
}


int8_t gpio_set_pin_configs(struct PinConfig* pin_config) {


  for (int i = 0; i < pin_config->len; i++) {
    uint16_t pin = pin_config->pin_config[i].pin;
    gpio_set_mode(pin, pin_config->pin_config[i].function);
    if (pin_config->pin_config[i].function != GPIO_MODE_AF) continue;
    gpio_set_af(pin, pin_config->pin_config[i].af_mode);

  }

  *pin_config->rcc |= BIT(pin_config->pos);

  return 0;
}

static inline void gpio_init_pwm(uint16_t pin, uint16_t prescaler, uint16_t auto_reload) {
  struct tim* timer = pin_timer_configs[pin].timer;
  if (!timer) return;

  
  volatile uint32_t* afr = (PINNO(pin) <= 7) ? &GPIO(PINBANK(pin))->AFR[0] : &GPIO(PINBANK(pin))->AFR[1];
  *afr |= (pin_timer_configs[pin].af << 4 * (PINNO(pin) - ((PINNO(pin) <= 7) ? 0 : 8)) ); 
  

  volatile uint32_t* rcc_ptr = pin_timer_configs[pin].rcc_ptr;
  *rcc_ptr |= (1 << pin_timer_configs[pin].rcc_bit_pos);
  
  timer->PSC = prescaler;
  timer->ARR = auto_reload;
  
  switch (pin_timer_configs[pin].channel)
  {
    case 1 : 
      ((union CCMR*)(&timer->CCMR1))->fields.oc1m = 0b110;
      ((union CCER*)(&timer->CCER))->fields.cc1e = 1;
      break;
    case 2 : 
      ((union CCMR*)(&timer->CCMR1))->fields.oc2m = 0b110;
      ((union CCER*)(&timer->CCER))->fields.cc2e = 1;
      break;
    case 3 : 
      ((union CCMR*)(&timer->CCMR2))->fields.oc1m = 0b110;
      ((union CCER*)(&timer->CCER))->fields.cc3e = 1;
      break;
    case 4 : 
      ((union CCMR*)(&timer->CCMR2))->fields.oc2m = 0b110;
      ((union CCER*)(&timer->CCER))->fields.cc4e = 1;
      break;
    default : break;
  }
  if (pin_timer_configs[pin].advanced)
    ((union BDTR*)(&timer->BDTR))->fields.moe = 1;
  
  ((union CR1*)(&timer->CR1))->fields.cen = 1;

  pin_timer_configs[pin].enable = 1;
}

static inline void gpio_write(uint16_t pin, uint16_t value) {
  struct gpio *gpio = GPIO(PINBANK(pin));

  if (!pin_timer_configs[pin].enable) {
    if (value == GPIO_TOGGLE) {
      gpio->ODR ^= (1U << PINNO(pin));
    } else {
      gpio->BSRR = (1U << PINNO(pin)) << (value ? 0 : 16);
    }
  } else {
    switch(pin_timer_configs[pin].channel) {
      case 1 : pin_timer_configs[pin].timer->CCR1 = value; break;
      case 2 : pin_timer_configs[pin].timer->CCR2 = value; break;
      case 3 : pin_timer_configs[pin].timer->CCR3 = value; break; 
      case 4 : pin_timer_configs[pin].timer->CCR4 = value; break;
      default : break;
    }
  }
}

static inline uint8_t gpio_read(uint16_t pin) {
  struct gpio *gpio = GPIO(PINBANK(pin));

  return (gpio->IDR & BIT(PINNO(pin)))? 0x01 : 0x00;
}

#endif