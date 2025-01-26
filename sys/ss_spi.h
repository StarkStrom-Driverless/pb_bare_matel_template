#ifndef _SS_SPI_H_
#define _SS_SPI_H_

#include "ss_makros.h"
#include "ss_pin.h"
#include "ss_spi_def.h"
#include "ss_rcc_def.h"

// MISO - MOSI - SCK - NSS
struct PinConfig pin_config_spi1 = {
    {{PIN('A', 6), GPIO_MODE_AF, 5}, {PIN('A', 7), GPIO_MODE_AF, 5}, {PIN('A', 5), GPIO_MODE_AF, 5}, {PIN('A', 4), GPIO_MODE_AF, 5} },
    4, 
    (volatile uint32_t*)&RCC->APB2ENR,
    12
};

struct PinConfig pin_config_spi2 = {
    {{PIN('B', 14), GPIO_MODE_AF, 5}, {PIN('B', 15), GPIO_MODE_AF, 5}, {PIN('B', 13), GPIO_MODE_AF, 5}, {PIN('B', 12), GPIO_MODE_AF, 5} },
    4,
    (volatile uint32_t*)&RCC->APB1ENR,
    14
};

struct PinConfig pin_config_spi3 = {
    {{PIN('C', 11), GPIO_MODE_AF, 6}, {PIN('C', 12), GPIO_MODE_AF, 6}, {PIN('C', 10), GPIO_MODE_AF, 6}, {PIN('A', 15), GPIO_MODE_AF, 6} },
    4,
    (volatile uint32_t*)&RCC->APB1ENR,
    15
};

struct PinConfig* get_spi_pins(uint8_t spi_id) {
    struct PinConfig* pin_config = 0;
    switch(spi_id) {
        case 1: pin_config = &pin_config_spi1; break;
        case 2: pin_config = &pin_config_spi2; break;
        case 3: pin_config = &pin_config_spi3; break;
        default: break;
    }
    return pin_config;
}

void SPI_Init(struct PinConfig* pin_config, struct spi* spi) {
    (void)pin_config;
    /*
    uint16_t miso = pin_config->pin_config[0].pin;
    uint16_t mosi = pin_config->pin_config[1].pin;
    uint16_t sck = pin_config->pin_config[2].pin;
*/
    if (spi == SPI1) {
        RCC->APB2ENR |= BIT(12);
    } else if(spi == SPI2) {
        RCC->APB1ENR |= BIT(14);
    } else if(spi == SPI3) {
        RCC->AHB1ENR |= BIT(15);
    } else {
        return;
    }

    /*
    volatile uint32_t* afr = (PINNO(miso) <= 7) ? &GPIO(PINBANK(miso))->AFR[0] : &GPIO(PINBANK(miso))->AFR[1]; 
    *afr |= (9 << 4 * (PINNO(miso) - ((PINNO(miso) <= 7) ? 0 : 8)));

    afr = (PINNO(mosi) <= 7) ? &GPIO(PINBANK(mosi))->AFR[0] : &GPIO(PINBANK(mosi))->AFR[1]; 
    *afr |= (9 << 4 * (PINNO(mosi) - ((PINNO(mosi) <= 7) ? 0 : 8)));

    afr = (PINNO(sck) <= 7) ? &GPIO(PINBANK(sck))->AFR[0] : &GPIO(PINBANK(sck))->AFR[1]; 
    *afr |= (9 << 4 * (PINNO(sck) - ((PINNO(sck) <= 7) ? 0 : 8)));
    */

    union SPI_CR1* spi_cr1 = (union SPI_CR1*)(&spi->spi_cr1);

    spi_cr1->spi_cr1 = 0;
    spi_cr1->fields.mstr = 1;
    spi_cr1->fields.br = 0;
    spi_cr1->fields.ssm = 1;
    spi_cr1->fields.cpha = 1;
    spi_cr1->fields.spe = 1;
}

uint8_t spi_exchange_data(uint8_t data, struct spi* spi) {
    union SPI_SR* spi_sr = (union SPI_SR*)(&spi->spi_sr);
    
    while(spi_sr->fields.txe == 0);
    spi->spi_dr = data;

    while(spi_sr->fields.rxne == 0);
    return (uint8_t)spi->spi_dr;
}

#endif