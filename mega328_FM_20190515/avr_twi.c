#include "avr_twi.h"
#include <stdio.h>

#define LOCAL_DEBUG
#ifdef LOCAL_DEBUG
#define LOG(args) (printf("TWI "), printf args)
#endif

uint8_t twi_start(void)
{
	uint8_t ret = 0;
	TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN);	// I2C start
	while (!(TWCR & (1<<TWINT)));           	// I2C wait
	
	// check value of TWI Status Register. Mask prescaler bits.
	uint8_t twst = TW_STATUS & 0xF8;
	if ( (twst != TW_START) && (twst != TW_REP_START))
	{
		LOG(("Write error @START, twst = %02X\n", twst));
		ret = 1;
	}
	return ret;
}


uint8_t twi_write_address(uint8_t val)
{
	uint8_t ret = 0;
	TWDR = val;                            	// send address
	TWCR = (1<<TWINT) | (1<<TWEN);
	while (!(TWCR & (1<<TWINT)));
	
	uint8_t twst = TW_STATUS & 0xF8;
	if ( (twst != TW_MT_SLA_ACK) && (twst != TW_MR_SLA_ACK) )
	{
		LOG(("Write error @ADDR, twst = %02X\n", twst));
		ret = 1;	
	}
	return ret;
}

uint8_t twi_write_data(uint8_t val)
{
	uint8_t ret = 0;
	TWDR = val;                 
	TWCR = (1<<TWINT) | (1<<TWEN);      
	while (!(TWCR & (1<<TWINT)));
	// check value of TWI Status Register. Mask prescaler bits
	uint8_t twst = TW_STATUS & 0xF8;
	if( twst != TW_MT_DATA_ACK)
	{
		LOG(("Write error, twst = %02X\n", twst));
		ret = 1;
	}
	return ret;
}

void twi_stop(void)
{
	TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWSTO);	// I2C stop
	// wait until stop condition is executed and bus released
	while(TWCR & (1<<TWSTO));
}
