/*
 * Code generated from Atmel Start.
 *
 * This file will be overwritten when reconfiguring your Atmel Start project.
 * Please copy examples or other code you want to keep to a separate file
 * to avoid losing it when reconfiguring.
 */
#ifndef ATMEL_START_PINS_H_INCLUDED
#define ATMEL_START_PINS_H_INCLUDED

#include <hal_gpio.h>

// SAMV71 has 4 pin functions

#define GPIO_PIN_FUNCTION_A 0
#define GPIO_PIN_FUNCTION_B 1
#define GPIO_PIN_FUNCTION_C 2
#define GPIO_PIN_FUNCTION_D 3

#define PA3 GPIO(GPIO_PORTA, 3)
#define PA4 GPIO(GPIO_PORTA, 4)
#define PA21 GPIO(GPIO_PORTA, 21)
#define PB4 GPIO(GPIO_PORTB, 4)

/* LED pin to make the onboard led blink */
#define LED0 GPIO(GPIO_PORTA, 23)

#endif // ATMEL_START_PINS_H_INCLUDED
