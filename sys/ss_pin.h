#include <stdint.h>

#ifndef _SS_PIN_H_
#define _SS_PIN_H_

enum { GPIO_MODE_INPUT, GPIO_MODE_OUTPUT, GPIO_MODE_AF, GPIO_MODE_ANALOG };


struct Pin {
    uint16_t pin;
    uint8_t function;
    uint8_t af_mode;
};

struct PinConfig {
    struct Pin pin_config[10];
    uint8_t len;
    volatile uint32_t *rcc;
    uint8_t pos;
};

#endif