#include "radio_config.h"
#include <avr/crc16.h>		
#include <avr/eeprom.h>
#include <stdio.h>
#include <string.h>


#define LOCAL_DEBUG
#ifdef LOCAL_DEBUG
#define LOG(args) (printf("CONFIG "), printf args)
#endif


struct Config config;

enum { LOCATION1 = 100, LOCATION2 = 200 };

static uint16_t current_addr = LOCATION2;

static uint16_t calculate_crc(struct Config *config)
{
	uint16_t crc = 0xFFFF;
	for (uint8_t i=0; i<offsetof(struct Config, crc); i++)
	{
		crc = _crc16_update(crc, *(((unsigned char *)config)+i));
	}
	return crc;
}

static int load_config_from(uint16_t addr, struct Config* const cfg)
{
	eeprom_read_block(cfg, (unsigned int *)addr, sizeof(*cfg));
	uint16_t crc = calculate_crc(cfg);
	if (crc == cfg->crc)
		return 0;
	return -1;
}

void load_config(void)
{
	struct Config tmp;
	int8_t loc1valid = 0, loc2valid = 0;
	if (load_config_from(LOCATION1, &tmp) == 0)
	{
		loc1valid = 1;
	}
	if (load_config_from(LOCATION2, &config) == 0)
	{
		loc2valid = 1;
	}
	
	if (!loc1valid && !loc2valid)
	{
		// default config
		memset(&config, 0, sizeof(config));
		
		config.id = 0;
		config.on = 1;
		config.frequency = 9990;
		config.volume = CONFIG_DEFAULT_VOLUME;
		config.stereo = 1;
		current_addr = LOCATION2;
		LOG(("No config found, using default\n"));
		return;
	}
	else if (loc1valid && loc2valid)
	{
		// both locations valid -> compare ids
		if ((config.id - tmp.id) & 0x8000)
		{
			config = tmp;
			current_addr = LOCATION1;			
		}
		else if (tmp.id > config.id)
		{
			config = tmp;
			current_addr = LOCATION1;
		}
	}
	else if (loc1valid && !loc2valid)
	{
		config = tmp;
		current_addr = LOCATION1;
	}
	if (config.volume < CONFIG_MIN_VOLUME || config.volume > CONFIG_MAX_VOLUME)
	{
		config.volume = CONFIG_DEFAULT_VOLUME;
	}
	LOG(("Config loaded from addr %d, freq = %u, vol = %u\n",
		current_addr, (unsigned int)(config.frequency/100), config.volume));
	config.id++;
}

void store_config(void)
{

	// swap address
	if (current_addr == LOCATION2)
		current_addr = LOCATION1;
	else
		current_addr = LOCATION2;
	config.crc = calculate_crc(&config);
	eeprom_write_block(&config, (unsigned int *)current_addr, sizeof(config));
	LOG(("Config saved, id = %u\n", config.id));	
	// increment id to give priority to new location
	config.id++;
}
