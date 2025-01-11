#include <inttypes.h>

#ifndef _SS_SPI_DEF_H_
#define _SS_SPI_DEF_H_

struct spi {
    volatile uint32_t spi_cr1, spi_cr2, spi_sr, spi_dr, spi_crcpr, spi_rxcrcr, spi_txcrcr, spi_i2scfgr, spi_i2spr;
};

#define SPI1 ((struct spi*) 0x40013000)
#define SPI2 ((struct spi*) 0x40003800)
#define SPI3 ((struct spi*) 0x40003800)

union SPI_CR1 {
    struct SPI_CR1_FIELDS {
        volatile uint8_t cpha : 1;
        volatile uint8_t cpol : 1;
        volatile uint8_t mstr : 1;
        volatile uint8_t br : 3;
        volatile uint8_t spe : 1;
        volatile uint8_t lsbfirst : 1;
        volatile uint8_t ssi : 1;
        volatile uint8_t ssm : 1;
        volatile uint8_t rxonly : 1;
        volatile uint8_t dff : 1;
        volatile uint8_t crcnext : 1;
        volatile uint8_t crcen : 1;
        volatile uint8_t bidioe : 1;
        volatile uint8_t bidimode : 1;
        volatile uint16_t reserved : 16; 
    } fields;
    volatile uint32_t spi_cr1;
};

union SPI_SR {
    struct SPI_SR_FIELDS {
        volatile uint8_t rxne : 1;
        volatile uint8_t txe : 1;
        volatile uint8_t chside : 1;
        volatile uint8_t udr : 1;
        volatile uint8_t crcerr : 1;
        volatile uint8_t modf : 1;
        volatile uint8_t ovr : 1;
        volatile uint8_t bsy : 1;
        volatile uint8_t fre : 1;
        volatile uint32_t reserved : 24;
    }fields;
    volatile uint32_t spi_sr;
};



#endif