/*
 * Code generated from Atmel Start.
 *
 * This file will be overwritten when reconfiguring your Atmel Start project.
 * Please copy examples or other code you want to keep to a separate file
 * to avoid losing it when reconfiguring.
 */
#ifndef DRIVER_INIT_INCLUDED
#define DRIVER_INIT_INCLUDED

#include "atmel_start_pins.h"

#ifdef __cplusplus
extern "C" {
#endif

#include <hal_atomic.h>
#include <hal_delay.h>
#include <hal_gpio.h>
#include <hal_init.h>
#include <hal_io.h>
#include <hal_sleep.h>

#include <hal_usart_async.h>

#include <hal_i2c_m_async.h>

#include <hal_pwm.h>
#include <hpl_tcc.h>
#include <hal_can_async.h>

#include <hal_dac_sync.h>

extern struct usart_async_descriptor USART_Diag;

extern struct i2c_m_async_desc I2C;

extern struct pwm_descriptor       PWM_CTRL;
extern struct can_async_descriptor CAN_CTRL;

extern struct dac_sync_descriptor DAC;

void USART_Diag_PORT_init(void);
void USART_Diag_CLOCK_init(void);
void USART_Diag_init(void);

void I2C_PORT_init(void);
void I2C_CLOCK_init(void);
void I2C_init(void);

void PWM_CTRL_PORT_init(void);
void PWM_CTRL_CLOCK_init(void);
void PWM_CTRL_init(void);

void DAC_PORT_init(void);
void DAC_CLOCK_init(void);
void DAC_init(void);

/**
 * \brief Perform system initialization, initialize pins and clocks for
 * peripherals
 */
void system_init(void);

#ifdef __cplusplus
}
#endif
#endif // DRIVER_INIT_INCLUDED
