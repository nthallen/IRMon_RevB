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

// SAMC21 has 9 pin functions

#define GPIO_PIN_FUNCTION_A 0
#define GPIO_PIN_FUNCTION_B 1
#define GPIO_PIN_FUNCTION_C 2
#define GPIO_PIN_FUNCTION_D 3
#define GPIO_PIN_FUNCTION_E 4
#define GPIO_PIN_FUNCTION_F 5
#define GPIO_PIN_FUNCTION_G 6
#define GPIO_PIN_FUNCTION_H 7
#define GPIO_PIN_FUNCTION_I 8

#define IR GPIO(GPIO_PORTA, 2)
#define FTRX_SAMCTX GPIO(GPIO_PORTA, 4)
#define FTTX_SAMCRX GPIO(GPIO_PORTA, 5)
#define RSTB GPIO(GPIO_PORTA, 6)
#define SHDNB GPIO(GPIO_PORTA, 7)
#define SDA GPIO(GPIO_PORTA, 8)
#define SCL GPIO(GPIO_PORTA, 9)
#define PWM0 GPIO(GPIO_PORTA, 12)
#define PWM1 GPIO(GPIO_PORTA, 13)
#define ALRT GPIO(GPIO_PORTA, 15)
#define PA24 GPIO(GPIO_PORTA, 24)
#define PA25 GPIO(GPIO_PORTA, 25)
#define STLED GPIO(GPIO_PORTA, 27)

#endif // ATMEL_START_PINS_H_INCLUDED
