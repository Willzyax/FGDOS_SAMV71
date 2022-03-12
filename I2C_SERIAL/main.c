#include <atmel_start.h>
#include <stdio.h>

#define INA219_1_ADDRESS 0x40
#define INA219_2_ADDRESS 0x41

#define INA219_CONFIG_BVOLTAGERANGE_16V 0x0000 // 16 V bus range
#define INA219_CONFIG_GAIN_1_40MV 0x0000 // Gain 1, 40 mV
#define INA219_CONFIG_BADCRES_12BIT 0x0180 // 1 x 12-bit shunt sample
#define INA219_CONFIG_SADCRES_12BIT_1S_532US 0x0018 // 1 x 12-bit shunt sample
#define INA219_CONFIG_MODE_SANDBVOLT_CONTINUOUS 0x07 // < shunt and bus voltage continuous

#define INA219_REG_SHUNTVOLTAGE (0x01)
#define INA219_REG_BUSVOLTAGE (0x02)
#define INA219_REG_POWER (0x03)
#define INA219_REG_CURRENT (0x04)
#define INA219_REG_CONFIG 0x00
#define INA219_REG_CALIBRATION 0x05

#define INA219_VSHUNT_PGA1_MASK 0x8FFF
#define INA219_VBUS_MASK 0xFFF8

#define STR_DATA_SIZE 60
#define STR_INFO_SIZE 80

void retrieve_voltage(uint8_t sensor, uint16_t *V_bus, int16_t *V_shunt);

int main(void)
{
	/* Initializes MCU, drivers and middleware */
	atmel_start_init();

	/* initialize USART for PC communication, variable definitions */
	struct io_descriptor *io_serial;
	usart_sync_get_io_descriptor(&SERIAL, &io_serial);
	usart_sync_enable(&SERIAL);
	char str_data[STR_DATA_SIZE], str_info[STR_INFO_SIZE], vbus_str[10], vshunt_str[10];

	/* I2C (TWI) setup */
	struct io_descriptor *I2C_0_io;
	i2c_m_sync_get_io_descriptor(&I2C_0, &I2C_0_io);
	i2c_m_sync_enable(&I2C_0);
	int16_t V_shunt_1 = 0, V_shunt_2 = 0;
	uint16_t V_bus_1 = 0, V_bus_2 = 0;
	bool flag_shunt = true, flag_bus = true;
	/* set configuration register for both sensors
	 * also set the calibration register if you want to measure current and power directly
	 */
	uint16_t config =	INA219_CONFIG_BVOLTAGERANGE_16V |
		INA219_CONFIG_GAIN_1_40MV | INA219_CONFIG_BADCRES_12BIT |
		INA219_CONFIG_SADCRES_12BIT_1S_532US |
		INA219_CONFIG_MODE_SANDBVOLT_CONTINUOUS;
	i2c_m_sync_set_slaveaddr(&I2C_0, INA219_1_ADDRESS, I2C_M_SEVEN);
	i2c_m_sync_cmd_write(&I2C_0, INA219_REG_CONFIG, &config, 2);
	i2c_m_sync_set_slaveaddr(&I2C_0, INA219_2_ADDRESS, I2C_M_SEVEN);
	i2c_m_sync_cmd_write(&I2C_0, INA219_REG_CONFIG, &config, 2);
	
	/* print settings over serial */
	//snprintf(str_info, STR_INFO_SIZE,"V_s_1(microV),V_bus_1(milliV),V_shunt_2(microV),V_bus_2(milliV)\n");
	//io_write(io_serial, str_info, 100);
	//while(!usart_sync_is_tx_empty(io_serial)){}
	/* Replace with your application code */
	while (1) {	
		// sensor 1 & 2 measurements
		retrieve_voltage(INA219_1_ADDRESS,&V_bus_1,&V_shunt_1);
		retrieve_voltage(INA219_2_ADDRESS,&V_bus_2,&V_shunt_2);
		
		//delay_us(1); // otherwise the V values do not get updated in the background before sending them over serial, only needed when using 1 sensor
		
		snprintf(str_data, STR_DATA_SIZE,"Vs1(uV),%d,Vb1(mV),%d,Vs2(uV),%d,Vb2(mV),%d\n", V_shunt_1, V_bus_1, V_shunt_2,V_bus_2);
		io_write(io_serial, str_data, STR_DATA_SIZE);
		while(!usart_sync_is_tx_empty(io_serial)){}
		//note: when string is shorter than previous one, the end still contains \n and values! so reset string
		memset(str_data, 0, STR_DATA_SIZE);
	}
}

void retrieve_voltage(uint8_t address, uint16_t *V_bus, int16_t *V_shunt){
	i2c_m_sync_set_slaveaddr(&I2C_0, address, I2C_M_SEVEN);
	i2c_m_sync_cmd_read(&I2C_0, INA219_REG_SHUNTVOLTAGE, V_shunt, 2);
	delay_us(1); // otherwise bytes are not correctly registered
	*V_shunt = ((((*V_shunt & 0x00FF) << 8) | ((*V_shunt & 0xFF00) >> 8)) & INA219_VSHUNT_PGA1_MASK)  *10; // put MSByte first and convert to µV
	//*V_shunt = (*V_shunt & INA219_VSHUNT_PGA1_MASK)*10;
	i2c_m_sync_cmd_read(&I2C_0, INA219_REG_BUSVOLTAGE, V_bus, 2);
	delay_us(1);
	*V_bus = ((((*V_bus & 0x00FF) << 8) | ((*V_bus & 0xFF00) >> 8)) >> 3) *4; // put MSByte first and convert to mV
	//*V_bus = (*V_bus >> 3) *4;
}


/*
* older codes
		//while(flag_shunt){flag_shunt = false;}
		//flag_shunt = true;
		
		//while(flag_shunt){flag_bus = false;}
		//flag_bus = true;

		//itoa(V_bus,vbus_str,10);
		//itoa(V_shunt,vshunt_str,10);

		//gpio_toggle_pin_level(LED0);
		//delay_ms(1000);
*/