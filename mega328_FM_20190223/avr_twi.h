#ifndef AVR_TWI_H
#define AVR_TWI_H

#include <util/twi.h>
#include <stdint.h>

uint8_t twi_start(void);

uint8_t twi_write_address(uint8_t val);

uint8_t twi_write_data(uint8_t val);

void twi_stop(void);

//setup the I2C hardware to ACK the next transmission
//and indicate that we've handled the last one.
#define TWACK (TWCR=(1<<TWINT)|(1<<TWEN)|(1<<TWEA))
//setup the I2C hardware to NACK the next transmission
#define TWNACK (TWCR=(1<<TWINT)|(1<<TWEN)) 

#endif
