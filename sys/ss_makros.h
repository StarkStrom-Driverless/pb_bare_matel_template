#ifndef _MAKROS_H_
#define _MAKROS_H_

#define BIT(x) (1UL << (x))
#define PIN(bank, num) ((((bank) - 'A') << 4) | (num))
#define PINNO(pin) (pin & 15)
#define PINBANK(pin) (pin >> 4)

#endif