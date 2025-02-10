#include "ss_gpio.h"
#include "ss_spi.h"
#include "ads131m04_def.h"
#include "ss_delay.h"

#ifndef _ADS131M04_H_
#define _ADS131M04_H_


uint16_t ads_ext_i;
uint16_t ads_rst_o;

void init_ads_pins() {
    ads_ext_i = PIN('A', 4);
    ads_rst_o = PIN('A', 3);

    gpio_set_mode(ads_ext_i, GPIO_MODE_INPUT);
    gpio_set_mode(ads_rst_o, GPIO_MODE_OUTPUT);

    gpio_write(ads_rst_o, GPIO_ON);
}

uint8_t ads_write_register(uint8_t address, uint16_t value) {
    uint16_t res;
    uint8_t addressRcv;
    uint8_t bytesRcv;
    uint16_t cmd = 0;

    cmd = CMD_READ_REG | ((uint16_t)address << 7);

    delay(16);

    spi_exchange_data16(1, cmd);
    spi_exchange_data(1, 0x00);

    spi_exchange_data16(1, value);
    spi_exchange_data(1, 0x00);

    spi_exchange_data16(1, 0x0000);
    spi_exchange_data(1, 0x00);

    spi_exchange_data16(1, 0x0000);
    spi_exchange_data(1, 0x00);

    spi_exchange_data16(1, 0x0000);
    spi_exchange_data(1, 0x00);

    spi_exchange_data16(1, 0x0000);
    spi_exchange_data(1, 0x00);

    spi_exchange_data16(1, 0x0000);
    spi_exchange_data(1, 0x00);

    res = spi_exchange_data16(1, 0x0000);
    spi_exchange_data(1, 0x00);

    spi_exchange_data16(1, 0x0000);
    spi_exchange_data(1, 0x00);

    spi_exchange_data16(1, 0x0000);
    spi_exchange_data(1, 0x00);

    spi_exchange_data16(1, 0x0000);
    spi_exchange_data(1, 0x00);

    spi_exchange_data16(1, 0x0000);
    spi_exchange_data(1, 0x00);

    spi_exchange_data16(1, 0x0000);
    spi_exchange_data(1, 0x00);

    delay(16);

    addressRcv = (res & REGMASK_CMD_READ_REG_ADDRESS) >> 7;
    bytesRcv = (res & REGMASK_CMD_READ_REG_BYTES);

    if (addressRcv == address)
    {
        return bytesRcv + 1;
    }
    return 0;
}

uint16_t ads_read_register(uint8_t address) {
    uint16_t cmd;
    uint16_t data;

    cmd = CMD_READ_REG | (address << 7 | 0);

    delay(16);

    spi_exchange_data16(1, cmd);
    spi_exchange_data(1, 0x00);

    spi_exchange_data16(1, 0x0000);
    spi_exchange_data(1, 0x00);

    spi_exchange_data16(1, 0x0000);
    spi_exchange_data(1, 0x00);

    spi_exchange_data16(1, 0x0000);
    spi_exchange_data(1, 0x00);

    spi_exchange_data16(1, 0x0000);
    spi_exchange_data(1, 0x00);

    spi_exchange_data16(1, 0x0000);
    spi_exchange_data(1, 0x00);

    data = spi_exchange_data16(1, 0x0000);
    spi_exchange_data(1, 0x00);

    spi_exchange_data16(1, 0x0000);
    spi_exchange_data(1, 0x00);

    spi_exchange_data16(1, 0x0000);
    spi_exchange_data(1, 0x00);

    spi_exchange_data16(1, 0x0000);
    spi_exchange_data(1, 0x00);

    spi_exchange_data16(1, 0x0000);
    spi_exchange_data(1, 0x00);

    spi_exchange_data16(1, 0x0000);
    spi_exchange_data(1, 0x00);
    
    delay(16);

    return data;
}

uint8_t ads_write_register_masked(uint8_t address, uint16_t value, uint16_t mask) {
    uint16_t register_content = ads_read_register(address);
    register_content = register_content & ~mask;
    register_content = register_content | value;
    ads_write_register(address, register_content);
    return 0;
}

uint8_t ads_set_osr(uint16_t osr) {
    if (osr > 7) return -1;
    ads_write_register_masked(REG_CLOCK, osr << 2, REGMASK_CLOCK_OSR);
    return 0;
}

int8_t init_ads131() {
    int8_t ret = 0;
    init_ads_pins();
    ret = SPI_Init(1, 4000000);
    ads_set_osr(OSR_128);
    return ret;
}

int8_t set_input_channel_section(uint8_t channel, uint8_t input) {
    int8_t ret = 0;
    switch (channel) {
        case 0: ads_write_register_masked(REG_CH0_CFG, input, REGMASK_CHX_CFG_MUX); break;
        case 1: ads_write_register_masked(REG_CH0_CFG, input, REGMASK_CHX_CFG_MUX); break;
        case 2: ads_write_register_masked(REG_CH0_CFG, input, REGMASK_CHX_CFG_MUX); break;
        case 3: ads_write_register_masked(REG_CH0_CFG, input, REGMASK_CHX_CFG_MUX); break;
        default: ret = -1; break;
    }
    return ret;
}

int8_t is_data_ready_soft(uint8_t channel) {
    int ret = -1;
    uint16_t status = ads_read_register(REG_STATUS);

    switch (channel)
    {
        case  0: ret = (status & REGMASK_STATUS_DRDY0); break;
        case  1: ret = (status & REGMASK_STATUS_DRDY0); break;
        case  2: ret = (status & REGMASK_STATUS_DRDY0); break;
        case  3: ret = (status & REGMASK_STATUS_DRDY0); break;
        default: break;
    }
    
    return ret;
}

uint8_t ads_is_data_ready() {
    uint8_t val = gpio_read(ads_ext_i);
    return (val)?0:1;
}



int8_t ads_read(struct adcOutput* ads_output_ptr) {
    uint8_t x = 0;
    uint8_t x2 = 0;
    uint8_t x3 = 0;
    int32_t aux;

    delay(16);

    x = spi_exchange_data(1, 0x00);
    x2 =  spi_exchange_data(1, 0x00);
    spi_exchange_data(1, 0x00);

    ads_output_ptr->status = ((x << 8) | x2);

    for (uint8_t i = 0; i < 4; i++) {
        x = spi_exchange_data(1, 0x00);
        x2 = spi_exchange_data(1, 0x00);
        x3 = spi_exchange_data(1, 0x00);
        aux = (((x << 16) | (x2 << 8) | x3) & 0x00FFFFFF);
        if (aux > 0x7FFFFF) {
            ads_output_ptr->ch[i] = ((~(aux)&0x00FFFFFF) + 1) * -1;
        } else {
            ads_output_ptr->ch[i] = aux;
        }
    }

    delay(16);

    return 0;
}



#endif