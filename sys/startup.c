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
extern void CAN1_RX0_IRQHandler(void);
extern void CAN2_RX0_IRQHandler(void);
extern void DefaultHandler(void);




// 16 standard and 91 STM32-specific handlers
__attribute__((section(".vectors"))) void (*const tab[16 + 91])(void) = {
    _estack,                        // 0
    _reset,                         // 1
    DefaultHandler,                              // 2
    DefaultHandler,                              // 3
    DefaultHandler,                              // 4
    DefaultHandler,                              // 5
    DefaultHandler,                              // 6
    DefaultHandler,                              // 7
    DefaultHandler,                              // 8
    DefaultHandler,                              // 9
    DefaultHandler,                              // 10
    DefaultHandler,                              // 11
    DefaultHandler,                              // 12
    DefaultHandler,                              // 13
    DefaultHandler,                              // 14
    SysTick_Handler,                             // 15
    DefaultHandler,                              // 0
    DefaultHandler,                              // 1
    DefaultHandler,                              // 2
    DefaultHandler,                              // 3
    DefaultHandler,                              // 4
    DefaultHandler,                              // 5
    DefaultHandler,                              // 6
    DefaultHandler,                              // 7
    DefaultHandler,                              // 8
    DefaultHandler,                              // 9
    DefaultHandler,                              // 10
    DefaultHandler,                              // 11
    DefaultHandler,                              // 12
    DefaultHandler,                              // 13
    DefaultHandler,                              // 14
    DefaultHandler,                              // 15
    DefaultHandler,                              // 16
    DefaultHandler,                              // 17
    DefaultHandler,                              // 18
    DefaultHandler,                              // 19
    CAN1_RX0_IRQHandler,                         // 20
    DefaultHandler,                             // 21
    DefaultHandler,                             // 22
    DefaultHandler,                              // 23
    DefaultHandler,                              // 24
    DefaultHandler,                              // 25
    DefaultHandler,                              // 26
    DefaultHandler,                              // 27
    DefaultHandler,                              // 28
    DefaultHandler,                              // 29
    DefaultHandler,                              // 30
    DefaultHandler,                              // 31
    DefaultHandler,                              // 32
    DefaultHandler,                              // 33
    DefaultHandler,                              // 34
    DefaultHandler,                              // 35
    DefaultHandler,                              // 36
    DefaultHandler,                              // 37
    DefaultHandler,                              // 38
    DefaultHandler,                              // 39
    DefaultHandler,                              // 40
    DefaultHandler,                              // 41
    DefaultHandler,                              // 42
    DefaultHandler,                              // 43
    DefaultHandler,                              // 44
    DefaultHandler,                              // 45
    DefaultHandler,                              // 46
    DefaultHandler,                              // 47
    DefaultHandler,                              // 48
    DefaultHandler,                              // 49
    DefaultHandler,                              // 50
    DefaultHandler,                              // 51
    DefaultHandler,                              // 52
    DefaultHandler,                              // 53
    DefaultHandler,                              // 54
    DefaultHandler,                              // 55
    DefaultHandler,                              // 56
    DefaultHandler,                              // 57
    DefaultHandler,                              // 58
    DefaultHandler,                              // 59
    DefaultHandler,                              // 60
    DefaultHandler,                              // 61
    DefaultHandler,                              // 62
    DefaultHandler,                              // 63
    CAN2_RX0_IRQHandler,                         // 64
};