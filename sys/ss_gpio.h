#include "ss_makros.h"
#include "ss_gpio_def.h"
#include "ss_rcc_def.h"
#include "ss_timer_def.h"

#ifndef _GPIO_H
#define _GPIO_H

enum {GPIO_ON, GPIO_OFF, GPIO_TOGGLE, GPIO_PWM};

// Enum values are per datasheet: 0, 1, 2, 3
enum { GPIO_MODE_INPUT, GPIO_MODE_OUTPUT, GPIO_MODE_AF, GPIO_MODE_ANALOG };

static inline void gpio_set_mode(uint16_t pin, uint8_t mode) {
  struct gpio *gpio = GPIO(PINBANK(pin));
  int n = PINNO(pin);
  gpio->MODER &= ~(3U << (n * 2));
  gpio->MODER |= (mode & 3U) << (n * 2);
}

static inline void gpio_init_pwm(uint16_t pin, uint16_t prescaler, uint16_t auto_reload) {
  struct tim* timer = pin_timer_configs[pin].timer;
  if (!timer) return;

  
  volatile uint32_t* afr = (PINNO(pin) <= 7) ? &GPIO(PINBANK(pin))->AFR[0] : &GPIO(PINBANK(pin))->AFR[1];
  switch (PINNO(pin)) {
    case 0 : ((union AFR*)afr)->fields.afr0 = pin_timer_configs[pin].af; break;
    case 1 : ((union AFR*)afr)->fields.afr1 = pin_timer_configs[pin].af; break;
    case 2 : ((union AFR*)afr)->fields.afr2 = pin_timer_configs[pin].af; break;
    case 3 : ((union AFR*)afr)->fields.afr3 = pin_timer_configs[pin].af; break;
    case 4 : ((union AFR*)afr)->fields.afr4 = pin_timer_configs[pin].af; break;
    case 5 : ((union AFR*)afr)->fields.afr5 = pin_timer_configs[pin].af; break;
    case 6 : ((union AFR*)afr)->fields.afr6 = pin_timer_configs[pin].af; break;
    case 7 : ((union AFR*)afr)->fields.afr7 = pin_timer_configs[pin].af; break;
    default : break;
  }
  
  
  volatile uint32_t* rcc_ptr = pin_timer_configs[pin].rcc_ptr;
  *rcc_ptr |= (1 << pin_timer_configs[pin].rcc_bit_pos);
 
  timer->PSC = prescaler;
  timer->ARR = auto_reload;

/*  
  ((union PRC*)(&(timer->PSC)))->fields.psc = prescaler;
  ((union ARR*)(&(timer->ARR)))->fields.arr = auto_reload;
  

  volatile uint32_t* psc = (volatile uint32_t*)(0x40000C00 + 0x28);
  *psc = prescaler;

  volatile uint32_t* arr = (volatile uint32_t*)(0x40000C00 + 0x2c);
  *arr = auto_reload;
*/
  
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
  

 /*
  volatile uint32_t* ccmr = (volatile uint32_t*)(0x40000C00 + 0x1c);
  *ccmr |= (1 << 6) | (1 << 5);

  volatile uint32_t* ccer = (volatile uint32_t*)(0x40000C00 + 0x20);
  *ccer |= (1 << 8); 
*/
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

#endif