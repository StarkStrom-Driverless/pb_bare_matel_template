//#include "ss_can.h"

// Startup code
__attribute__((naked, noreturn)) void _reset(void) {
    // memset .bss to zero, and copy .data section to RAM region
    extern long _sbss, _ebss, _sdata, _edata, _sidata;
    for (long *dst = &_sbss; dst < &_ebss; dst++) *dst = 0;
    for (long *dst = &_sdata, *src = &_sidata; dst < &_edata;) *dst++ = *src++;
    extern void main(void);
    main();             // Call main()
    for (;;) (void) 0;  // Infinite loop in the case if main() returns
}

extern void _estack(void);
extern void SysTick_Handler(void);
//extern void CAN1_RX0_IRQHandler(void);

// 16 standard and 91 STM32-specific handlers
__attribute__((section(".vectors"))) void (*const tab[16 + 91])(void) = {
    _estack,                        // 0
    _reset,                         // 1
    0,                              // 2
    0,                              // 3
    0,                              // 4
    0,                              // 5
    0,                              // 6
    0,                              // 7
    0,                              // 8
    0,                              // 9
    0,                              // 10
    0,                              // 11
    0,                              // 12
    0,                              // 13
    0,                              // 14
    SysTick_Handler,                // 15
    0,                              // 0
    0,                              // 1
    0,                              // 2
    0,                              // 3
    0,                              // 4
    0,                              // 5
    0,                              // 6
    0,                              // 7
    0,                              // 8
    0,                              // 9
    0,                              // 10
    0,                              // 11
    0,                              // 12
    0,                              // 13
    0,                              // 14
    0,                              // 15
    0,                              // 16
    0,                              // 17
    0,                              // 18
    0,                              // 19
    0//CAN1_RX0_IRQHandler             // 20
};