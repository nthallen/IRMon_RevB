/*
 * Code generated from Atmel Start.
 *
 * This file will be overwritten when reconfiguring your Atmel Start project.
 * Please copy examples or other code you want to keep to a separate file
 * to avoid losing it when reconfiguring.
 */

#include "driver_init.h"
#include <peripheral_clk_config.h>
#include <utils.h>
#include <hal_init.h>

/*! The buffer size for USART */
#define USART_DIAG_BUFFER_SIZE 16

struct usart_async_descriptor USART_Diag;
struct can_async_descriptor   CAN_CTRL;

static uint8_t USART_Diag_buffer[USART_DIAG_BUFFER_SIZE];

struct i2c_m_async_desc I2C;

struct pwm_descriptor PWM_CTRL;

struct dac_sync_descriptor DAC;

/**
 * \brief USART Clock initialization function
 *
 * Enables register interface and peripheral clock
 */
void USART_Diag_CLOCK_init()
{

	hri_gclk_write_PCHCTRL_reg(GCLK, SERCOM0_GCLK_ID_CORE, CONF_GCLK_SERCOM0_CORE_SRC | (1 << GCLK_PCHCTRL_CHEN_Pos));
	hri_gclk_write_PCHCTRL_reg(GCLK, SERCOM0_GCLK_ID_SLOW, CONF_GCLK_SERCOM0_SLOW_SRC | (1 << GCLK_PCHCTRL_CHEN_Pos));
	hri_mclk_set_APBCMASK_SERCOM0_bit(MCLK);
}

/**
 * \brief USART pinmux initialization function
 *
 * Set each required pin to USART functionality
 */
void USART_Diag_PORT_init()
{

	gpio_set_pin_function(UART_RX, PINMUX_PA04D_SERCOM0_PAD0);

	gpio_set_pin_function(UART_TX, PINMUX_PA10C_SERCOM0_PAD2);
}

/**
 * \brief USART initialization function
 *
 * Enables USART peripheral, clocks and initializes USART driver
 */
void USART_Diag_init(void)
{
	USART_Diag_CLOCK_init();
	usart_async_init(&USART_Diag, SERCOM0, USART_Diag_buffer, USART_DIAG_BUFFER_SIZE, (void *)NULL);
	USART_Diag_PORT_init();
}

void I2C_PORT_init(void)
{

	gpio_set_pin_pull_mode(SDA,
	                       // <y> Pull configuration
	                       // <id> pad_pull_config
	                       // <GPIO_PULL_OFF"> Off
	                       // <GPIO_PULL_UP"> Pull-up
	                       // <GPIO_PULL_DOWN"> Pull-down
	                       GPIO_PULL_OFF);

	gpio_set_pin_function(SDA, PINMUX_PA08D_SERCOM2_PAD0);

	gpio_set_pin_pull_mode(SCL,
	                       // <y> Pull configuration
	                       // <id> pad_pull_config
	                       // <GPIO_PULL_OFF"> Off
	                       // <GPIO_PULL_UP"> Pull-up
	                       // <GPIO_PULL_DOWN"> Pull-down
	                       GPIO_PULL_OFF);

	gpio_set_pin_function(SCL, PINMUX_PA09D_SERCOM2_PAD1);
}

void I2C_CLOCK_init(void)
{
	hri_gclk_write_PCHCTRL_reg(GCLK, SERCOM2_GCLK_ID_CORE, CONF_GCLK_SERCOM2_CORE_SRC | (1 << GCLK_PCHCTRL_CHEN_Pos));
	hri_gclk_write_PCHCTRL_reg(GCLK, SERCOM2_GCLK_ID_SLOW, CONF_GCLK_SERCOM2_SLOW_SRC | (1 << GCLK_PCHCTRL_CHEN_Pos));
	hri_mclk_set_APBCMASK_SERCOM2_bit(MCLK);
}

void I2C_init(void)
{
	I2C_CLOCK_init();
	i2c_m_async_init(&I2C, SERCOM2);
	I2C_PORT_init();
}

void PWM_CTRL_PORT_init(void)
{

	gpio_set_pin_function(PWM0, PINMUX_PA12E_TCC2_WO0);

	gpio_set_pin_function(PWM1, PINMUX_PA13E_TCC2_WO1);
}

void PWM_CTRL_CLOCK_init(void)
{
	hri_mclk_set_APBCMASK_TCC2_bit(MCLK);
	hri_gclk_write_PCHCTRL_reg(GCLK, TCC2_GCLK_ID, CONF_GCLK_TCC2_SRC | (1 << GCLK_PCHCTRL_CHEN_Pos));
}

void PWM_CTRL_init(void)
{
	PWM_CTRL_CLOCK_init();
	PWM_CTRL_PORT_init();
	pwm_init(&PWM_CTRL, TCC2, _tcc_get_pwm());
}

void CAN_CTRL_PORT_init(void)
{

	gpio_set_pin_function(CANRX, PINMUX_PA25G_CAN0_RX);

	gpio_set_pin_function(CANTX, PINMUX_PA24G_CAN0_TX);
}
/**
 * \brief CAN initialization function
 *
 * Enables CAN peripheral, clocks and initializes CAN driver
 */
void CAN_CTRL_init(void)
{
	hri_mclk_set_AHBMASK_CAN0_bit(MCLK);
	hri_gclk_write_PCHCTRL_reg(GCLK, CAN0_GCLK_ID, CONF_GCLK_CAN0_SRC | (1 << GCLK_PCHCTRL_CHEN_Pos));
	can_async_init(&CAN_CTRL, CAN0);
	CAN_CTRL_PORT_init();
}

void DAC_PORT_init(void)
{

	// Disable digital pin circuitry
	gpio_set_pin_direction(IR, GPIO_DIRECTION_OFF);

	gpio_set_pin_function(IR, PINMUX_PA02B_DAC_VOUT);
}

void DAC_CLOCK_init(void)
{

	hri_mclk_set_APBCMASK_DAC_bit(MCLK);
	hri_gclk_write_PCHCTRL_reg(GCLK, DAC_GCLK_ID, CONF_GCLK_DAC_SRC | (1 << GCLK_PCHCTRL_CHEN_Pos));
}

void DAC_init(void)
{
	DAC_CLOCK_init();
	dac_sync_init(&DAC, DAC);
	DAC_PORT_init();
}

void system_init(void)
{
	init_mcu();

	// GPIO on PA06

	gpio_set_pin_level(RSTB,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   true);

	// Set pin direction to output
	gpio_set_pin_direction(RSTB, GPIO_DIRECTION_OUT);

	gpio_set_pin_function(RSTB, GPIO_PIN_FUNCTION_OFF);

	// GPIO on PA07

	gpio_set_pin_level(SHDNB,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   true);

	// Set pin direction to output
	gpio_set_pin_direction(SHDNB, GPIO_DIRECTION_OUT);

	gpio_set_pin_function(SHDNB, GPIO_PIN_FUNCTION_OFF);

	// GPIO on PA15

	// Set pin direction to input
	gpio_set_pin_direction(ALRT, GPIO_DIRECTION_IN);

	gpio_set_pin_pull_mode(ALRT,
	                       // <y> Pull configuration
	                       // <id> pad_pull_config
	                       // <GPIO_PULL_OFF"> Off
	                       // <GPIO_PULL_UP"> Pull-up
	                       // <GPIO_PULL_DOWN"> Pull-down
	                       GPIO_PULL_OFF);

	gpio_set_pin_function(ALRT, GPIO_PIN_FUNCTION_OFF);

	// GPIO on PA27

	gpio_set_pin_level(STLED,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   false);

	// Set pin direction to output
	gpio_set_pin_direction(STLED, GPIO_DIRECTION_OUT);

	gpio_set_pin_function(STLED, GPIO_PIN_FUNCTION_OFF);

	USART_Diag_init();

	I2C_init();

	PWM_CTRL_init();
	CAN_CTRL_init();

	DAC_init();
}
