#include <inttypes.h>

#ifndef _SS_SPI_DEF_H_
#define _SS_SPI_DEF_H_

struct spi {
    volatile uint32_t spi_cr1, spi_cr2, spi_sr, spi_dr, spi_crcpr, spi_rxcrcr, spi_txcrcr, spi_i2scfgr, spi_i2spr;
};

#define SPI1 ((struct spi*) 0x40013000)
#define SPI2 ((struct spi*) 0x40003800)
#define SPI3 ((struct spi*) 0x40003800)
#define SPI4 ((struct spi*) 0x40013400)
#define SPI5 ((struct spi*) 0x40015000)
#define SPI6 ((struct spi*) 0x40015400)

#endif