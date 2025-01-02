#include <inttypes.h>
#include <stdbool.h>

#ifndef _GPIO_DEF_H_
#define _GPIO_DEF_H_

struct gpio {
  volatile uint32_t MODER, OTYPER, OSPEEDR, PUPDR, IDR, ODR, BSRR, LCKR, AFR[2];
};
#define GPIO(bank) ((struct gpio *) (0x40020000 + 0x400 * (bank)))

#endif