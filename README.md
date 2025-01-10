# ProcessorBoard Bare Matel Template

## OUTPUT
To set a pin to high or low just do the following
```c
int main(void) {
  uint16_t PA2 = PIN('A', 2);
  Usable GPIO_set_mode(PA2, Usable GPIO_MODE_OUTPUT);

  for (;;) {
      Usable GPIO_write(PA2, Usable GPIO_ON); //Usable GPIO_ON - Usable GPIO_OFF - Usable GPIO_TOGGLE
  }
  return 0;
}

```
- create a unique id for the pin 2 on port a
- set the mode of the pin to Usable GPIO_MODE_OUTPUT
- Use the Usable GPIO_write function to set the output value


## PWM
To use the pwm function of a pin just do the following
```c
int main(void) {
  uint16_t PA2 = PIN('A', 2);
  Usable GPIO_set_mode(PA2, Usable GPIO_MODE_AF);
  Usable GPIO_init_pwm(PA2, 159, 2000);

  for (;;) {
      Usable GPIO_write(PA2, 1000);
  }
  return 0;
}

```
- create a unique id for the pin 2 on port a
- set the mode of the pin to alternate function
- initialize the pin to pwm
    - Set the prescaler for the timer peripheral 
    - Set the auto reload value
- Use the Usable GPIO_write function to set the capture compare register. 
  This function is also used to set high or low in output mode. 
  It recognizes if the unique id is set to pwm or to output mode.
- The capture compare value has to be lower than the auto reload value


## SYSTICK HANDLING
To use the systick for executing commands in specific time intervalls just do this.

- Go to usr_configs/systick_handles.h
```c
#ifndef _SYSTICK_HANDLES_H_
#define _SYSTICK_HANDLES_H_

#include "ss_systick.h"

Systick_Handle handle1 = {.timer = 0, .period=10, .tick = 0};

#endif
```
- Initialize a struct with typ Systick_Handle
- the only value you have to adapt is the period value

- Go to usr_configs/systick_isr.h
```c
#ifndef _SYSTICK_ISR_H_
#define _SYSTICK_ISR_H_

#include "systick_handles.h"

extern Systick_Handle handle1;

void SysTick_Handler(void) {
  handle1.tick++;
}

#endif
```
- define the previous defined handle1 struct as extern
- in the SysTick_Handler ISR-Function increase the tick variable of the handler struct

- Go to main
```c
int main(void) {
  uint16_t led = PIN('A', 2);
  
  Usable GPIO_set_mode(led, Usable GPIO_MODE_OUTPUT);


  systick_init(16000000/100);

  uint16_t pwm_val = 0;

  for (;;) {
    if (handle_timer(&handle1)) {
      Usable GPIO_write(led, Usable GPIO_TOGGLE);
    }
  }
  return 0;
}
```

# PIN capabilities
## PORTA
|PIN|Usable GPIO|PWM|CAN|SPI|
|-|-|-|-|-|
|PA0|True|True|||
|PA1|True|True|||
|PA2|True|True|||
|PA3|True|True|||
|PA4|True|||SPI1_NSS - SPI3_NSS|
|PA5|True|True||SPI1_SCK|
|PA6|True|True||SPI1_MISO|
|PA7|True|True||SPI1_MOSI|
|PA8|True|True|||
|PA9|True|True|||
|PA10|True|True|||
|PA11|True|True|||
|PA12|True||||
|PA13|||||
|PA14|||||
|PA15|True|True||SPI1_NSS - SPI3_NSS|

## PORTB
|PIN|Usable GPIO|PWM|CAN|SPI|
|-|-|-|-|-|
|PB0|True|True|||
|PB1|True|True|||
|PB2|||||
|PB3||||SPI1_SCK - SPI3_SCK|
|PB4|||STB2|SPI1_MISO - SPI3_MISO|
|PB5|||RX2|SPI1_MOSI - SPI3_MOSI|
|PB6|||TX2||
|PB7|||STB1||
|PB8|||RX1||
|PB9|||TX1|SPI2_NSS|
|PB10|True|True||SPI2_SCK|
|PB11|True|True|||
|PB12|True|||SPI2_NSS|
|PB13|True|||SPI2_SCK|
|PB14|True|True||SPI2_MISO|
|PB15|True|True||SPI2_MOSI|

## PORTC
|PIN|Usable GPIO|PWM|CAN|SPI|
|-|-|-|-|-|
|PC0|||||
|PC1|||||
|PC2|True||||
|PC3|True||||
|PC4|||||
|PC5|||||
|PC6|True|True|||
|PC7|True|True|||
|PC8|True|True|||
|PC9|True|True|||
|PC10|True|||SPI3_SCK|
|PC11|True|||SPI3_MISO|
|PC12|True|||SPI3_MOSI|
|PC13|True||||
|PC14|True||||
|PC15|True||||

