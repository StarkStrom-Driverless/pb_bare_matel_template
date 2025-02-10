#ifndef _SS_SPI_H_
#define _SS_SPI_H_

#include "ss_makros.h"
#include "ss_pin.h"
#include "ss_spi_def.h"
#include "ss_rcc_def.h"



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

struct spi* get_spi_ptr(uint8_t spi_id) {
    struct spi* spi_ptr = 0;
    switch(spi_id) {
        case 1: spi_ptr = SPI1; break;
        case 2: spi_ptr = SPI2; break;
        case 3: spi_ptr = SPI3; break;
        default: break;
    }
    return spi_ptr;
}

int16_t get_spi_br(uint32_t bitrate) {
    uint32_t spix_clk = 16000000;
    uint32_t br_value = spix_clk / bitrate;
    if (br_value < 2 || br_value > 256) {
        return -1;  // Ungültige Bitrate
    }

    switch (br_value) {
        case 2:
            return 0;
        case 4:
            return 1;
        case 8:
            return 2;
        case 16:
            return 3;
        case 32:
            return 4;
        case 64:
            return 5;
        case 128:
            return 6;
        case 256:
            return 7;
        default:
            return -1;
    }
}

int8_t SPI_Init(uint8_t spi_id, uint32_t bitrate) {
    (void) bitrate;

    if (gpio_set_pin_configs(get_spi_pins(spi_id)) == -1) return -1;

    struct spi* spi = get_spi_ptr(spi_id);

    union SPI_CR1* spi_cr1 = (union SPI_CR1*)(&spi->spi_cr1);

    int16_t br = get_spi_br(bitrate);
    if (br == -1) return -1;

    spi_cr1->spi_cr1 = 0;
    spi_cr1->fields.mstr = 1;
    spi_cr1->fields.br = (uint8_t)(br & 0x7);;
    spi_cr1->fields.ssm = 1;
    spi_cr1->fields.cpha = 1;
    spi_cr1->fields.spe = 1;

    return 0;
}

uint8_t spi_exchange_data(uint8_t spi_id, uint8_t data) {
    struct spi* spi = get_spi_ptr(spi_id);
    union SPI_SR* spi_sr = (union SPI_SR*)(&spi->spi_sr);
    
    spi->spi_cr1 &= ~(uint32_t)(1 << 11);
    
    while (spi_sr->fields.txe == 0);
    spi->spi_dr = data;
    
    while (spi_sr->fields.rxne == 0);
    return (uint8_t)spi->spi_dr;
}

uint16_t spi_exchange_data16(uint8_t spi_id, uint16_t data) {
    struct spi* spi = get_spi_ptr(spi_id);
    union SPI_SR* spi_sr = (union SPI_SR*)(&spi->spi_sr);
    
    spi->spi_cr1 |= (1 << 11);
    
    while (spi_sr->fields.txe == 0);
    spi->spi_dr = data;
    
    while (spi_sr->fields.rxne == 0);
    return (uint16_t)spi->spi_dr;
}


#endif