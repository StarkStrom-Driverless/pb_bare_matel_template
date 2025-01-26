#include <inttypes.h>
#include "ss_makros.h"
#include "ss_pin.h"
#include "ss_rcc_def.h"

#ifndef _SS_UART_DEF_H_
#define _SS_UART_DEF_H_

struct uart {
  volatile uint32_t SR, DR, BRR, CR1, CR2, CR3, GTPR;
};

#define UART1 ((struct uart*) 0x40011000)
#define UART2 ((struct uart*) 0x40004400)
#define UART3 ((struct uart*) 0x40004800)
#define UART4 ((struct uart*) 0x40004C00)
#define UART5 ((struct uart*) 0x40005000)
#define UART6 ((struct uart*) 0x40011400)
#define UART7 ((struct uart*) 0x40007800)
#define UART8 ((struct uart*) 0x40007C00)



struct PinConfig pin_config_uart1 = {
    {{PIN('A', 9), GPIO_MODE_AF, 7}, {PIN('A', 10), GPIO_MODE_AF, 7}},
    2,
    (volatile uint32_t*)&RCC->APB2ENR,
    4
};

struct PinConfig pin_config_uart2 = {
    {{PIN('A', 3), GPIO_MODE_AF, 7}, {PIN('A', 2), GPIO_MODE_AF, 7}},
    2,
    (volatile uint32_t*)&RCC->APB1ENR,
    17
};

struct PinConfig pin_config_uart3 = {
    {{PIN('B', 10), GPIO_MODE_AF, 7}, {PIN('B', 11), GPIO_MODE_AF, 7}},
    2,
    (volatile uint32_t*)&RCC->APB1ENR,
    18
};

struct PinConfig pin_config_uart4 = {
    {{PIN('C', 11), GPIO_MODE_AF, 8}, {PIN('B', 10), GPIO_MODE_AF, 8}},
    2,
    (volatile uint32_t*)&RCC->APB1ENR,
    19
};

struct PinConfig pin_config_uart5 = {
    {{PIN('C', 12), GPIO_MODE_AF, 8}, {PIN('D', 2), GPIO_MODE_AF, 8}},
    2,
    (volatile uint32_t*)&RCC->APB1ENR,
    19
};

struct PinConfig pin_config_uart6 = {
    {{PIN('C', 7), GPIO_MODE_AF, 8}, {PIN('C', 6), GPIO_MODE_AF, 8}},
    2,
    (volatile uint32_t*)&RCC->APB2ENR,
    5
};


#endif