#include <inttypes.h>
#include "ss_makros.h"
#include "ss_gpio.h"

#ifndef _PIN_DECLARATIONS_H_
#define _PIN_DECLARATIONS_H_

/***
 * Processorboard Specific definitions
 */

uint16_t pin_heartbeat;
uint16_t pin_error;
uint16_t pin_blue_one;
uint16_t pin_blue_two;

uint16_t can1_tx;
uint16_t can1_rx;
uint16_t can1_stb;

void inti_pb_pins(void) {
    // LEDS
    pin_heartbeat = PIN('C', 1);
    pin_error = PIN('C', 0);
    pin_blue_one = PIN('C', 4);
    pin_blue_two = PIN('C', '5');

    gpio_set_mode(pin_heartbeat, GPIO_MODE_OUTPUT);
    gpio_set_mode(pin_error, GPIO_MODE_OUTPUT);
    gpio_set_mode(pin_blue_one, GPIO_MODE_OUTPUT);
    gpio_set_mode(pin_blue_two, GPIO_MODE_OUTPUT);

    gpio_write(pin_heartbeat, GPIO_ON);
    gpio_write(pin_error, GPIO_ON);
    gpio_write(pin_blue_one, GPIO_ON);
    gpio_write(pin_blue_two, GPIO_ON);

    //CAN
    can1_tx = PIN('B', 9);
    can1_rx = PIN('B', 8);
    can1_stb = PIN('B', 7);

    gpio_set_mode(can1_rx, GPIO_MODE_AF);
    gpio_set_mode(can1_tx, GPIO_MODE_AF);
    gpio_set_mode(can1_stb, GPIO_MODE_OUTPUT);

    gpio_write(can1_stb, GPIO_ON);
}

#endif