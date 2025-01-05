#ifndef _SS_CAN_H_
#define _SS_CAN_H_

#include <stdint.h>
#include "pin_declarations.h"
#include "ss_gpio.h"
#include "ss_makros.h"
#include "ss_can_def.h"
#include "ss_rcc_def.h"
#include "systick_handles.h"
#include "ss_systick.h"

extern Systick_Handle handle2;

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


void CAN_Init(uint16_t tx_pin, uint16_t rx_pin) {
    //FÜR später
    //SystemClock_Config();

    RCC->APB1ENR |= (1 << 25);

    volatile uint32_t* afr = (PINNO(tx_pin) <= 7) ? &GPIO(PINBANK(tx_pin))->AFR[0] : &GPIO(PINBANK(tx_pin))->AFR[1]; 
    *afr |= (9 << 4 * (PINNO(tx_pin) - ((PINNO(tx_pin) <= 7) ? 0 : 8)));
    afr = (PINNO(rx_pin) <= 7) ? &GPIO(PINBANK(rx_pin))->AFR[0] : &GPIO(PINBANK(rx_pin))->AFR[1]; 
    *afr |= (9 << 4 * (PINNO(rx_pin) - ((PINNO(rx_pin) <= 7) ? 0 : 8)));

    GPIO(PINBANK(tx_pin))->OSPEEDR = (uint32_t)((1 << PINNO(tx_pin)) | (1 << PINNO(rx_pin)));


    GPIO(PINBANK(rx_pin))->OSPEEDR |= (uint32_t)((1 << PINNO(tx_pin)) | (1 << PINNO(rx_pin)));


    union CAN_MCR* can_mcr = ((union CAN_MCR*)(&CAN1->CAN_MCR));
    union CAN_MSR* can_msr = ((union CAN_MSR*)(&CAN1->CAN_MSR));
    
    
    can_mcr->fields.inrq = 1;
    while(can_msr->fields.inak == 0);
    
    can_mcr->fields.sleep = 0;
    while(can_msr->fields.slak == 1);

    can_mcr->fields.abom = 1;


    CAN1->CAN_FMR |= (1 << 0);
    CAN1->CAN_FA1R &= ~(uint32_t)(1 << 0);
    CAN1->CAN_FS1R |= (1 << 0);
    CAN1->CAN_FM1R &= ~(uint32_t)(1 << 0);
    CAN1->CAN_FnRx[0][0] = 0x00000000;
    CAN1->CAN_FnRx[0][1] = 0x00000000;
    CAN1->CAN_FA1R |= (1 << 0);
    CAN1->CAN_FMR &= ~(uint32_t)(1 << 0);


    // Das geht
    CAN1->CAN_BTR |= 0;

    CAN1->CAN_BTR &= (uint32_t)~(0xF << 16);
    CAN1->CAN_BTR |= (9 << 16);

    CAN1->CAN_BTR &= (uint32_t)~(0x7 << 20);
    CAN1->CAN_BTR |= (4 << 20);


    //Das nicht
    //union CAN_BTR* can_btr = ((union CAN_BTR*)(&CAN1->CAN_BTR)); -- Definiert in ss_can_def.h

    //can_btr->fields.brp = 5;
    //can_btr->fields.ts1 = 10;
    //can_btr->fields.ts2 = 5;
    //can_btr->fields.sjw = 1;

    can_mcr->fields.inrq = 0;
    while(can_msr->fields.inak == 1);

    //Für später - Muss warscheinlich PRIO in NVIC gepackt noch werden
    //CAN_IER |= (1 << 1); // FIFO 0 message pending interrupt
    //NVIC_ISER0 |= (1 << 20); // Enable CAN1_RX0 interrupt in NVIC
}

void CAN_Send(uint32_t id, uint8_t *data, uint8_t len) {

   while( ((union CAN_TSR*)(&CAN1->CAN_TSR))->fields.tme == 0 ) {
        if (handle_timer(&handle2)) {
            gpio_write(pin_error, GPIO_TOGGLE);
        }
   }

   CAN1->CAN_TI0R = (id << 21);
   CAN1->CAN_TDT0R = len;
   CAN1->CAN_TDL0R = *(uint32_t *)data;
   CAN1->CAN_TDH0R = *(uint32_t *)(data + 4);


    ((union CAN_TI0R*)(&CAN1->CAN_TI0R))->fields.txrq = 1;

}

void CAN1_RX0_IRQHandler(void) {
    if (CAN1->CAN_RF0R & (0x3 << 0)) {
        uint32_t id = (CAN1->CAN_RI0R >> 21) & 0x7FF;
        uint8_t len = CAN1->CAN_RDT0R & 0xF;
        uint8_t data[8];
        *(uint32_t *)data = CAN1->CAN_RDL0R;
        *(uint32_t *)(data + 4) = CAN1->CAN_RDH0R;

        len += 0;
        id += 0;

        gpio_write(pin_heartbeat, GPIO_TOGGLE);

        CAN1->CAN_RF0R |= (1 << 5);
    }
}

#endif 