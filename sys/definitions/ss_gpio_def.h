#include <inttypes.h>
#include <stdbool.h>

#ifndef _GPIO_DEF_H_
#define _GPIO_DEF_H_

struct gpio {
  volatile uint32_t MODER, OTYPER, OSPEEDR, PUPDR, IDR, ODR, BSRR, LCKR, AFR[2];
};
#define GPIO(bank) ((struct gpio *) (0x40020000 + 0x400 * (bank)))

union AFR {
  struct FIELDS_AFR {
    volatile uint8_t afr0 : 4;
    volatile uint8_t afr1 : 4;
    volatile uint8_t afr2 : 4;
    volatile uint8_t afr3 : 4;
    volatile uint8_t afr4 : 4;
    volatile uint8_t afr5 : 4;
    volatile uint8_t afr6 : 4;
    volatile uint8_t afr7 : 4;
  }fields;
  volatile uint32_t afr;
};

#endif