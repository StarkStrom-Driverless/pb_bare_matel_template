#include "ss_makros.h"
#include "ss_gpio_def.h"
#include "ss_rcc_def.h"

#ifndef _GPIO_H
#define _GPIO_H

enum {GPIO_ON, GPIO_OFF, GPIO_TOGGLE, GPIO_PWM};

// Enum values are per datasheet: 0, 1, 2, 3
enum { GPIO_MODE_INPUT, GPIO_MODE_OUTPUT, GPIO_MODE_AF, GPIO_MODE_ANALOG };

static inline void gpio_set_mode(uint16_t pin, uint8_t mode) {
  struct gpio *gpio = GPIO(PINBANK(pin));  // GPIO bank
  int n = PINNO(pin);                      // Pin number
  gpio->MODER &= ~(3U << (n * 2));         // Clear existing setting
  gpio->MODER |= (mode & 3U) << (n * 2);   // Set new mode
}

static inline void gpio_write(uint16_t pin, uint8_t mode, uint8_t pwm) {
  struct gpio *gpio = GPIO(PINBANK(pin));
  if (mode == GPIO_ON || mode == GPIO_OFF) {
    gpio->BSRR = (1U << PINNO(pin)) << (mode ? 0 : 16);
  } else if(mode == GPIO_TOGGLE) {
    gpio->ODR ^= (1U << PINNO(pin));
  } else if(mode == GPIO_PWM) {
    (void) pwm;
  } else{
    // should never be reached
  }
  
}

#endif