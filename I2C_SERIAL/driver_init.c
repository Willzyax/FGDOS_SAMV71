/*
 * Code generated from Atmel Start.
 *
 * This file will be overwritten when reconfiguring your Atmel Start project.
 * Please copy examples or other code you want to keep to a separate file
 * to avoid losing it when reconfiguring.
 */

#include "driver_init.h"
#include <hal_init.h>
#include <hpl_pmc.h>
#include <peripheral_clk_config.h>
#include <utils.h>
#include <hpl_usart_base.h>

struct i2c_m_sync_desc I2C_0;

struct usart_sync_descriptor SERIAL;

struct wdt_descriptor WDT_0;

void I2C_0_PORT_init(void)
{

	gpio_set_pin_function(PA4, MUX_PA4A_TWIHS0_TWCK0);

	gpio_set_pin_function(PA3, MUX_PA3A_TWIHS0_TWD0);
}

void I2C_0_CLOCK_init(void)
{
	_pmc_enable_periph_clock(ID_TWIHS0);
}

void I2C_0_init(void)
{
	I2C_0_CLOCK_init();

	i2c_m_sync_init(&I2C_0, TWIHS0);

	I2C_0_PORT_init();
}

void delay_driver_init(void)
{
	delay_init(SysTick);
}

void SERIAL_PORT_init(void)
{

	gpio_set_pin_function(PA21, MUX_PA21A_USART1_RXD1);

	gpio_set_pin_function(PB4, MUX_PB4D_USART1_TXD1);
}

void SERIAL_CLOCK_init(void)
{
	_pmc_enable_periph_clock(ID_USART1);
}

void SERIAL_init(void)
{
	SERIAL_CLOCK_init();
	SERIAL_PORT_init();
	usart_sync_init(&SERIAL, USART1, _usart_get_usart_sync());
}

void WDT_0_init(void)
{
	wdt_init(&WDT_0, WDT);
}

void LED_0_init(void)
{
	/* GPIO on PA23 */
	gpio_set_pin_level(LED0,
	// <y> Initial level
	// <id> pad_initial_level
	// <false"> Low
	// <true"> High
	true);
	// Set pin direction to output
	gpio_set_pin_direction(LED0, GPIO_DIRECTION_OUT);
	gpio_set_pin_function(LED0, GPIO_PIN_FUNCTION_OFF);
}

void system_init(void)
{
	init_mcu();

	I2C_0_init();

	delay_driver_init();

	SERIAL_init();

	//WDT_0_init();
	hri_wdt_set_MR_WDDIS_bit(WDT); // to disable the watchdog

	LED_0_init();
	
}
