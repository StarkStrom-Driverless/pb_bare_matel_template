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

#define CAN1_BASE       0x40006400U
#define CAN_TSR         (*(volatile uint32_t *)(CAN1_BASE + 0x08))
#define CAN_TI0R        (*(volatile uint32_t *)(CAN1_BASE + 0x180))
#define CAN_TDT0R       (*(volatile uint32_t *)(CAN1_BASE + 0x184))
#define CAN_TDL0R       (*(volatile uint32_t *)(CAN1_BASE + 0x188))
#define CAN_TDH0R       (*(volatile uint32_t *)(CAN1_BASE + 0x18C))
#define CAN_RF0R        (*(volatile uint32_t *)(CAN1_BASE + 0x0C))
#define CAN_RI0R        (*(volatile uint32_t *)(CAN1_BASE + 0x1B0))
#define CAN_RDT0R       (*(volatile uint32_t *)(CAN1_BASE + 0x1B4))
#define CAN_RDL0R       (*(volatile uint32_t *)(CAN1_BASE + 0x1B8))
#define CAN_RDH0R       (*(volatile uint32_t *)(CAN1_BASE + 0x1BC))
#define CAN_IER         (*(volatile uint32_t *)(CAN1_BASE + 0x14))
#define CAN_TSR_TME0    (1 << 26)



#define RCC_BASE        0x40023800
#define FLASH_BASE      0x40023C00
#define GPIOB_BASE      0x40020400

#define RCC_CR          (*(volatile uint32_t *)(RCC_BASE + 0x00))
#define RCC_PLLCFGR     (*(volatile uint32_t *)(RCC_BASE + 0x04))
#define RCC_CFGR        (*(volatile uint32_t *)(RCC_BASE + 0x08))
#define RCC_AHB1ENR     (*(volatile uint32_t *)(RCC_BASE + 0x30))
#define RCC_APB1ENR     (*(volatile uint32_t *)(RCC_BASE + 0x40))

#define FLASH_ACR       (*(volatile uint32_t *)(FLASH_BASE + 0x00))

#define GPIOB_MODER     (*(volatile uint32_t *)(GPIOB_BASE + 0x00))
#define GPIOB_AFRL      (*(volatile uint32_t *)(GPIOB_BASE + 0x20))

#define CAN1_MCR        (*(volatile uint32_t *)(CAN1_BASE + 0x00))
#define CAN1_MSR        (*(volatile uint32_t *)(CAN1_BASE + 0x04))
#define CAN1_BTR        (*(volatile uint32_t *)(CAN1_BASE + 0x1C))


 enum { APB1_PRE = 5 /* AHB clock / 4 */, APB2_PRE = 4 /* AHB clock / 2 */ }; 
 enum { PLL_HSI = 16, PLL_M = 8, PLL_N = 180, PLL_P = 2 };  // Run at 180 Mhz 
 #define FLASH_LATENCY 5 
 #define SYS_FREQUENCY ((PLL_HSI * PLL_N / PLL_M / PLL_P) * 1000000) 
 #define APB2_FREQUENCY (SYS_FREQUENCY / (BIT(APB2_PRE - 3))) 
 #define APB1_FREQUENCY (SYS_FREQUENCY / (BIT(APB1_PRE - 3))) 


void SystemClock_Config(void) {

    /*
    union PLLCFGR* pllcfgr = ((union PLLCFGR*)(&RCC->PLLCFGR));
    union CR* cr = ((union CR*)(&RCC->CR));
    union CFGR* cfgr = ((union CFGR*)(&RCC->CFGR));
    

    pllcfgr->fields.pllp = 0;
    pllcfgr->fields.pllm = PLL_M;
    pllcfgr->fields.plln = PLL_N;

    cr->fields.pllon = 1;
    while(cr->fields.pllrdy == 0);

    

    cfgr->fields.ppre1 = APB1_PRE;
    cfgr->fields.ppre2 = APB2_PRE;
    cfgr->fields.sw = 2;
    while(cfgr->fields.sws == 0);

    gpio_write(pin_error, GPIO_OFF);

    RCC->APB2ENR |= (1 << 14);
    */

   volatile uint32_t* cpacr = (volatile uint32_t*)(0xE000ED88);
   volatile uint32_t* acr = (volatile uint32_t*)(0x40023C00);    

   *cpacr |= ((3UL << 10 * 2) | (3UL << 11 * 2));
   *acr |= FLASH_LATENCY | BIT(8) | BIT(9);

   RCC->PLLCFGR &= ~((BIT(17) - 1));                   // Clear PLL multipliers 
   RCC->PLLCFGR |= (((PLL_P - 2) / 2) & 3) << 16;      // Set PLL_P 
   RCC->PLLCFGR |= PLL_M | (PLL_N << 6);               // Set PLL_M and PLL_N 
   RCC->CR |= BIT(24);                                 // Enable PLL 
   while ((RCC->CR & BIT(25)) == 0);           // Wait until done 
   RCC->CFGR = (APB1_PRE << 10) | (APB2_PRE << 13);    // Set prescalers 
   RCC->CFGR |= 2;                                     // Set clock source to PLL 
   while ((RCC->CFGR & 12) == 0);              // Wait until done 
  
   RCC->APB2ENR |= (1 << 14);    // Enable SYSCFG 
}


void CAN_Init(uint16_t tx_pin, uint16_t rx_pin) {

    

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
    //union CAN_BTR* can_btr = ((union CAN_BTR*)(&CAN1->CAN_BTR));
    
    can_mcr->fields.inrq = 1;
    while(can_msr->fields.inak == 0);
    
    can_mcr->fields.sleep = 0;
    while(can_msr->fields.slak == 1);

    

    can_mcr->fields.abom = 1;

    CAN1->CAN_BTR |= 0;

    CAN1->CAN_BTR &= (uint32_t)~(0xF << 16);
    CAN1->CAN_BTR |= (9 << 16);

    CAN1->CAN_BTR &= (uint32_t)~(0x7 << 20);
    CAN1->CAN_BTR |= (4 << 20);

    //can_btr->fields.brp = 5;
    //can_btr->fields.ts1 = 10;
    //can_btr->fields.ts2 = 5;
    //can_btr->fields.sjw = 1;

    can_mcr->fields.inrq = 0;
    while(can_msr->fields.inak == 1);

    

    


    //CAN_IER |= (1 << 1); // FIFO 0 message pending interrupt
    //NVIC_ISER0 |= (1 << 20); // Enable CAN1_RX0 interrupt in NVIC
}

void CAN_Send(uint32_t id, uint8_t *data, uint8_t len) {

    while (!(CAN_TSR & CAN_TSR_TME0)) {
        if (handle_timer(&handle2)) {
            gpio_write(pin_error, GPIO_TOGGLE);
        }
    }

    CAN_TI0R = (id << 21); // Set ID (standard ID, no extended ID)
    CAN_TDT0R = len;       // Set DLC (data length)
    CAN_TDL0R = *(uint32_t *)data; // Copy first 4 bytes of data
    CAN_TDH0R = *(uint32_t *)(data + 4); // Copy last 4 bytes of data

    CAN_TI0R |= (1 << 0); // Request transmission
    
}

void CAN1_RX0_IRQHandler(void) {
    /*
gpio_write(pin_heartbeat, GPIO_TOGGLE);

    if (CAN_RF0R & CAN_RF0R_FMP0) {
        uint32_t id = (CAN_RI0R >> 21) & 0x7FF; // Read ID
        uint8_t len = CAN_RDT0R & 0xF;         // Read DLC
        uint8_t data[8];
        *(uint32_t *)data = CAN_RDL0R;         // Read first 4 bytes
        *(uint32_t *)(data + 4) = CAN_RDH0R;   // Read last 4 bytes

        len += 0;
        id += 0;

        

        // Process received data...

        CAN_RF0R |= (1 << 5); // Release FIFO 0 output mailbox
    }
    */
}

#endif 