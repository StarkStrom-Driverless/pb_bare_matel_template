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

    struct PinConfig* pin = get_spi_pins(spi_id);
    if (gpio_set_pin_configs(pin) == -1) return -1;

    GPIO(0)->OSPEEDR |= (3<<10)|(3<<12)|(3<<14);
/*
    struct spi* spi = get_spi_ptr(spi_id);

    union SPI_CR1* spi_cr1 = (union SPI_CR1*)(&spi->spi_cr1);

    int16_t br = get_spi_br(bitrate);
    if (br == -1) return -1;

    spi_cr1->spi_cr1 = 0;
    spi_cr1->fields.mstr = 1;
    spi_cr1->fields.br = (uint8_t)(br & 0x7);;
    spi_cr1->fields.ssm = 1;
    spi_cr1->fields.ssi = 1;
    spi_cr1->fields.cpha = 1;
    spi_cr1->fields.cpol = 1;
    spi_cr1->fields.spe = 1;
*/


    SPI1->spi_cr1 = 0;
    SPI1->spi_cr1 |= (1<<0)|(1<<1);   // CPOL=1, CPHA=1
    SPI1->spi_cr1 |= (1<<2);  // Master Mode
    SPI1->spi_cr1 |= (3<<3);  // BR[2:0] = 011: fPCLK/16, PCLK2 = 80MHz, SPI clk = 5MHz
    SPI1->spi_cr1 &= ~(1<<7);  // LSBFIRST = 0, MSB first
    SPI1->spi_cr1 |= (1<<8) | (1<<9);  // SSM=1, SSi=1 -> Software Slave Management
    SPI1->spi_cr1 &= ~(1<<10);  // RXONLY = 0, full-duplex
    SPI1->spi_cr1 &= ~(1<<11);  // DFF=0, 8 bit data
    //SPI1->spi_cr1 |= (1<<1) | (1<<0);

    SPI1->spi_cr1 |= (1<<6);

    return 0;
}



void spiSendReceiveArrays(uint8_t spi_id, uint8_t *dataTx, uint8_t *dataRx, uint16_t numberOfBytes) {
    struct spi* spi = get_spi_ptr(spi_id);
    //union SPI_SR* spi_sr = (union SPI_SR*)(&spi->spi_sr);

 
    uint16_t i;
    for (i = 0; i < numberOfBytes; i++) {

        while (!((SPI1->spi_sr)&(1<<1)));
        spi->spi_dr = dataTx[i];

        while (!((SPI1->spi_sr)&(1<<1)))

        
        while (((SPI1->spi_sr)&(1<<7)));
        dataRx[i] = (uint8_t)spi->spi_dr;
    }


    //while (spi_sr->fields.bsy == 1);
}



#endif