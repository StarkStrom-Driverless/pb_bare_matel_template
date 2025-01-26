#include <inttypes.h>
#include "ss_makros.h"
#include "ss_gpio.h"
#include "ss_uart_def.h"



#ifndef _SS_UART_H_
#define _SS_UART_H_


struct PinConfig* get_uart_pins(uint8_t uart_id) {
    struct PinConfig* pin_config = 0;
    switch (uart_id)
    {
        case 1: pin_config = &pin_config_uart1; break;
        case 2: pin_config = &pin_config_uart2; break;
        case 3: pin_config = &pin_config_uart3; break;
        case 4: pin_config = &pin_config_uart4; break;
        case 5: pin_config = &pin_config_uart5; break;
        case 6: pin_config = &pin_config_uart6; break;
        default:break;
    }
    return pin_config;
}

struct uart* get_uart_ptr(uint8_t uart_id) {
    struct uart* uart_ptr = 0;
    switch (uart_id)
    {
        case 1: uart_ptr = UART1; break;
        case 2: uart_ptr = UART2; break;
        case 3: uart_ptr = UART3; break;
        case 4: uart_ptr = UART4; break;
        case 5: uart_ptr = UART5; break;
        case 6: uart_ptr = UART6; break;
        default:break;
    }
    return uart_ptr;
}

int8_t uart_init(uint8_t uart_id, unsigned long baud) {

    struct uart * uart = get_uart_ptr(uart_id);
    (void) uart_id;

    if (gpio_set_pin_configs(get_uart_pins(uart_id)) == -1) return -1;

    uart->CR1 = 0;
    uart->BRR = 16000000 / baud;
    uart->CR1 |= BIT(13) | BIT(2) | BIT(3);
    return 0;
}


static inline void uart_write_byte(struct uart *uart, uint8_t byte) {
    uart->DR = byte;
    while ((uart->SR & BIT(7)) == 0);
}

static inline void uart_write_buf(uint8_t uart_id, char *buf, uint8_t len) {
    struct uart* uart = get_uart_ptr(uart_id);
    while (len-- > 0) uart_write_byte(uart, *(uint8_t *) buf++);
}



#endif