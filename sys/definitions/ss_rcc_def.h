#include <inttypes.h>
#include <stdbool.h>

#ifndef _RCC_DEF_H_
#define _RCC_DEF_H_

struct rcc {
  volatile uint32_t CR, PLLCFGR, CFGR, CIR, AHB1RSTR, AHB2RSTR, AHB3RSTR,
      RESERVED0, APB1RSTR, APB2RSTR, RESERVED1[2], AHB1ENR, AHB2ENR, AHB3ENR,
      RESERVED2, APB1ENR, APB2ENR, RESERVED3[2], AHB1LPENR, AHB2LPENR,
      AHB3LPENR, RESERVED4, APB1LPENR, APB2LPENR, RESERVED5[2], BDCR, CSR,
      RESERVED6[2], SSCGR, PLLI2SCFGR;
};
#define RCC ((struct rcc *) 0x40023800)

union PLLCFGR {
  struct PLLCFGR_FIELDS {
    volatile uint8_t pllm : 6;
    volatile uint16_t plln : 9;
    volatile uint8_t reserved_1 : 1;
    volatile uint8_t pllp : 2;
    volatile uint8_t reserved_2 : 4;
    volatile uint8_t pllsrc : 1;
    volatile uint8_t reserved_3 : 4;
    volatile uint8_t pllq : 4;
    volatile uint8_t reserved_4 : 4;
  } fields;
  volatile uint32_t pllcfgr;
};

union CR {
  struct CR_FIELDS {
    volatile uint8_t hsion : 1;
    volatile uint8_t hsirdy : 1;
    volatile uint8_t reserved_1 : 1;
    volatile uint8_t hsitrim : 5;
    volatile uint8_t hsical : 8;
    volatile uint8_t hseon : 1;
    volatile uint8_t hserdy : 1;
    volatile uint8_t hsebyp : 1;
    volatile uint8_t csson : 1;
    volatile uint8_t reserved_2 : 4;
    volatile uint8_t pllon : 1;
    volatile uint8_t pllrdy : 1;
    volatile uint8_t plli2son : 1;
    volatile uint8_t plli2srdy : 1;
    volatile uint8_t reserved_3 : 4;
  } fields;
  volatile uint32_t cr;
};

union CFGR {
  struct CFGR_FIELDS {
    volatile uint8_t sw : 2;
    volatile uint8_t sws : 2;
    volatile uint8_t hpre : 4;
    volatile uint8_t reserved_1 : 2;
    volatile uint8_t ppre1 : 3;
    volatile uint8_t ppre2 : 3;
    volatile uint8_t rtcpre : 5;
    volatile uint8_t mco1 : 2;
    volatile uint8_t i2ssrc : 1;
    volatile uint8_t mco1pre : 3;
    volatile uint8_t mco2pre : 3;
    volatile uint8_t mco2 : 2;
  } fields;
  volatile uint32_t cfgr;
};



#endif