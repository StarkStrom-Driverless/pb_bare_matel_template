#include <stdint.h>
#include "ss_nvic.def.h"

#ifndef _SS_NVIC_H_
#define _SS_NVIC_H_

void enable_nvic_interrupt(int isr_number) {
    NVIC_ISR->iser[isr_number / 32] |= (1 << (isr_number % 32));
}

enum { APB1_PRE = 5 /* AHB clock / 4 */, APB2_PRE = 4 /* AHB clock / 2 */ }; 
enum { PLL_HSI = 16, PLL_M = 8, PLL_N = 180, PLL_P = 2 };  // Run at 180 Mhz 
#define FLASH_LATENCY 5 
#define SYS_FREQUENCY ((PLL_HSI * PLL_N / PLL_M / PLL_P) * 1000000) 
#define APB2_FREQUENCY (SYS_FREQUENCY / (BIT(APB2_PRE - 3))) 
#define APB1_FREQUENCY (SYS_FREQUENCY / (BIT(APB1_PRE - 3))) 




void SystemClock_Config(void) {

    volatile uint32_t* cpacr = (volatile uint32_t*)(0xE000ED88);
    volatile uint32_t* acr = (volatile uint32_t*)(0x40023C00);    

    union PLLCFGR* pllcfgr = ((union PLLCFGR*)(&RCC->PLLCFGR));
    union CR* cr = ((union CR*)(&RCC->CR));
    union CFGR* cfgr = ((union CFGR*)(&RCC->CFGR));
    

    *cpacr |= ((3UL << 10 * 2) | (3UL << 11 * 2));
    *acr |= FLASH_LATENCY | BIT(8) | BIT(9);


    pllcfgr->fields.pllp = 0;
    pllcfgr->fields.pllm = PLL_M;
    pllcfgr->fields.plln = PLL_N;

    cr->fields.pllon = 1;
    while(cr->fields.pllrdy == 0);

    

    cfgr->fields.ppre1 = APB1_PRE;
    cfgr->fields.ppre2 = APB2_PRE;
    cfgr->fields.sw = 2;
    while(cfgr->fields.sws == 0);

    RCC->APB2ENR |= (1 << 14);
    
}

#endif