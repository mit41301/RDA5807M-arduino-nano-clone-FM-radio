#ifndef RADIO_CONFIG_H_INCLUDED
#define RADIO_CONFIG_H_INCLUDED

#include <stdint.h>

enum {
	CONFIG_MIN_VOLUME = 1,
	CONFIG_DEFAULT_VOLUME = 8,
	CONFIG_MAX_VOLUME = 15
};

struct Config
{
	uint16_t id;		// 
	uint8_t on;			// default state after powering
	uint16_t frequency;	// unit = 10kHz
	uint8_t volume;
	uint8_t stereo;		// forcing mono if set to 0
	
	uint16_t station_memory[10];	// entries are not valid (not programmed) if equal to zero
	
	uint16_t crc;
};

extern struct Config config;

void load_config(void);

void store_config(void);

#endif
