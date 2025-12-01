/** FM radio based on RDA5807M module (native mode).
	Built with WinAVR 20100110
	
	Internal pull-ups are used with SDA (PC4) and SCL (PC5).
    As RDA5807M module operates with 3.3V logic, 3.3kOhm resistors are used in series with SDA and SCL
    for additional protection (if e.g. SDA/SCL would be accidentally configured in push-pull mode
    instead of weak internal pull-ups).
	
	IR receiver (VS1838B or similar) output is connected to PD2 / INT0 (see libnecdecoder.c).
*/

#include "main.h"
#include "RDA5807M.h"
#include "uart.h"
#include "radio_config.h"
#include "hw_config.h"
#include "timer1.h"
#include "libnecdecoder.h"
#include "ir_remote_s1_car_mp3.h"
#include "ui_state.h"
#include <avr/io.h>
#include <avr/delay.h>
#include <avr/signal.h>			// obsluga przerwan
#include <avr/interrupt.h>		// funkcje powiazane z obsluga przerwan
#include <avr/boot.h>			// reading fuses
#include <stdio.h>

#define LOCAL_DEBUG
#ifdef LOCAL_DEBUG
#	define LOG(args) (printf("MAIN "), printf args)
#else
#	define LOG(args)
#endif


#define ARRAY_SIZE(a) (sizeof(a) / sizeof(a[0]))


struct ui_state_s ui_state;


enum
{
	SLEEP_TIMER_PER_UNIT = 300UL * 1000 / MAIN_LOOP_DELAY
};

enum
{
	OFF_TIMER_PER_UNIT = 60UL * 1000 / MAIN_LOOP_DELAY
};

static struct sleep_state_s
{
	uint32_t timer;
} sleep_state;

static void init(void)    
{
	DDRB = 0XFF;
	// /** \note set minimum volume at startup */
	PORTB = 0x00;
	
	DDRC = 0B00000000;
	PORTC = 0Xff;	// internal pull-ups

	//_delay_ms(100);
	
	ui_state_reset(&ui_state);
	
	sleep_state.timer = 0;
	

	/** \note SCL = F_CPU/(16 + 2*TWBR*Prescaler) */
    TWSR = 0;                         /* no prescaler */	
	/** \note TWBR should be 10 or higher if the TWI operates in Master mode. If TWBR is lower than
	10, the Master may produce an incorrect output on SDA and SCL for the reminder of the
	byte. The problem occurs when operating the TWI in Master mode, sending Start + SLA
	+ R/W to a Slave (a Slave does not need to be connected to the bus for the condition to
	happen). */
	TWBR = 12;
	TWCR = (1<<TWEN); 

	UART_Init();

	ir_init();

	sei();							//globalne zezwolenie na przerwania
}

static void dump_fuses(void)
{
	uint8_t lfuse, hfuse, efuse, lock;

	cli();
	lfuse = boot_lock_fuse_bits_get(GET_LOW_FUSE_BITS);
	hfuse = boot_lock_fuse_bits_get(GET_HIGH_FUSE_BITS);
	efuse = boot_lock_fuse_bits_get(GET_EXTENDED_FUSE_BITS);
	lock = boot_lock_fuse_bits_get(GET_LOCK_BITS);
	sei();
	printf("LFUSE = %02X, HFUSE = %02X, EFUSE = %02X, LOCK = %02X\n", lfuse, hfuse, efuse, lock);
}

static void RDA5807M_start(void)
{
	RDA5807M_init();
	RDA5807M_tune(config.frequency);
	RDA5807M_set_stereo(config.stereo);
	_delay_ms(200);
	RDA5807M_set_volume(config.volume);
}

int main(void)
{
	uint8_t config_changed = 0;
	uint8_t searching = 0;
	struct RDA5807M_status status;
	uint8_t counter = 0;
	
	status.valid = 0;
	
	init();		
	
	LOG(("FM radio starting...\n"));

	dump_fuses();
	
	load_config();

	RDA5807M_start();
	if (!config.on)
	{
		RDA5807M_mute(true);
	}	
	
	while(1)
	{
		//LOG(("running...\n"));
		//printf(".");
		
		enum Command cmd = CMD__LIMITER;
		
		// IR control
		if(ir.status & (1<<IR_RECEIVED))
		{
			LOG(("NEC IR RX: address = %02X, command = %02X\n", ir.address, ir.command));
			cmd = ir_translate(ir.address, ir.command);
			LOG(("Command = %s (%d)\n", command_get_name(cmd), cmd));
			// Reset state
			ir.status &= ~(1<<IR_RECEIVED);
		}
		else
		{
			if (sleep_state.timer > 0)
			{
				sleep_state.timer--;
				if (sleep_state.timer == 0)
				{
					LOG(("sleep/wake timer elapsed\n"));
					cmd = CMD_ON_OFF;
				}
			}		
		}

		switch (cmd)
		{
			case CMD_ON_OFF:
			{
				if (config.on)
				{
					config.on = 0;
					RDA5807M_get_status(&status);
					//RDA5807M_shutdown();
					RDA5807M_mute(true);
				}
				else
				{
					config.on = 1;
					//RDA5807M_start();
					RDA5807M_mute(false);
				}			
				config_changed = 1;
				sleep_state.timer = 0;
			}
			break;
			
			case CMD_TUNE_DOWN:
			{
				RDA5807M_search(0);
				searching = 1;
				_delay_ms(500);
			}			
			break;
			
			case CMD_TUNE_UP:
			{
				RDA5807M_search(1);		
				searching = 2;
				_delay_ms(500);
			}
			break;
			
			case CMD_VOL_UP:
			{
				if (config.volume < CONFIG_MAX_VOLUME)
				{
					config.volume++;
					RDA5807M_set_volume(config.volume);
					config_changed = 1;
				}
			}
			break;
			
			case CMD_VOL_DOWN:
			{
				if (config.volume > CONFIG_MIN_VOLUME)
				{
					config.volume--;
					RDA5807M_set_volume(config.volume);
					config_changed = 1;
				}			
			}
			break;

			case CMD_STEREO_TOGGLE:
			{
				config.stereo = !config.stereo;
				RDA5807M_set_stereo(config.stereo);
				config_changed = 1;
			}
			break;
			
			case CMD_SLEEP_TIMER:
			{
				if (ui_state.st == UI_IDLE)
				{
					ui_set_state(&ui_state, UI_SLEEP);
				}
			}
			break;
			
			case CMD_CHANNEL_MEM_SET:
			{
				if (ui_state.st == UI_IDLE)
				{
					ui_set_state(&ui_state, UI_CHANNEL_SET_1);
				}
				else if (ui_state.st == UI_CHANNEL_SET_2)
				{
					// store channel memory
					if (ui_state.channel_mem_id < sizeof(config.station_memory)/sizeof(config.station_memory[0]))
					{
						LOG(("Setting station #%d\n", ui_state.channel_mem_id));
						config.station_memory[ui_state.channel_mem_id] = config.frequency;
						config_changed = 1;
					}					
				}
			}
			break;
			
			case CMD_0:
			case CMD_1:
			case CMD_2:
			case CMD_3:
			case CMD_4:
			case CMD_5:
			case CMD_6:
			case CMD_7:
			case CMD_8:
			case CMD_9:			
			{
				uint16_t val = cmd - CMD_0;
				switch(ui_state.st)
				{
				case UI_SLEEP:
					if (config.on)
					{
						sleep_state.timer = SLEEP_TIMER_PER_UNIT * val;
						LOG(("Setting sleep timer to %lu\n", sleep_state.timer));
					}
					else
					{
						sleep_state.timer = OFF_TIMER_PER_UNIT * val;
						LOG(("Setting wake timer to %lu\n", sleep_state.timer));
					}
					break;
				case UI_CHANNEL_SET_1:
					ui_set_state(&ui_state, UI_CHANNEL_SET_2);
					ui_state.channel_mem_id = val;
					break;
				case UI_IDLE:
					{
						if (val < sizeof(config.station_memory)/sizeof(config.station_memory[0]))
						{
							if (config.station_memory[val] != 0)
							{
								LOG(("Tuning to station #%d\n", val));
								config.frequency = config.station_memory[val];
								RDA5807M_tune(config.frequency);
								config_changed = 1;
							}
							else
							{
								LOG(("Station #%d is not configured\n", val));
							}
						}
					}
					break;
				default:
					LOG(("Unhandled number button (%d), ui_state.st = %d\n", val, ui_state.st));
					break;
				}
			}
			break;
			
			case CMD_CH_MINUS:
			{
				int index = 0;
				// find current station index
				for(int i=0; i<ARRAY_SIZE(config.station_memory); i++)
				{
					if (config.station_memory[i] == config.frequency)
					{
						index = i;
						break;
					}
				}
				LOG(("Current index = %d\n", index));
				// move to previous valid station index
				for(int i=0; i<ARRAY_SIZE(config.station_memory)-1; i++)
				{
					index--;
					if (index < 0)
					{
						index = ARRAY_SIZE(config.station_memory)-1;
					}
					if (config.station_memory[index] != 0)
					{
						LOG(("Tuning to station #%d\n", index));
						config.frequency = config.station_memory[index];
						RDA5807M_tune(config.frequency);
						config_changed = 1;
						break;
					}
				}
			}
			break;
			
			case CMD_CH_PLUS:
			{
				int index = ARRAY_SIZE(config.station_memory)-1;
				// find current station index
				for(int i=0; i<ARRAY_SIZE(config.station_memory); i++)
				{
					if (config.station_memory[i] == config.frequency)
					{
						index = i;
						break;
					}
				}
				LOG(("Current index = %d\n", index));
				// move to next station index
				for(int i=0; i<ARRAY_SIZE(config.station_memory)-1; i++)
				{
					index++;
					if (index >= ARRAY_SIZE(config.station_memory))
					{
						index = 0;
					}
					if (config.station_memory[index] != 0)
					{
						LOG(("Tuning to station #%d\n", index));
						config.frequency = config.station_memory[index];
						RDA5807M_tune(config.frequency);
						config_changed = 1;
						break;
					}
				}			
			}
			break;
			
			case CMD__LIMITER:
				// no command
			break;
			
			default:
			{
				LOG(("Unhandled command: %d\n", cmd));
			}
			break;			
		}

		
		_delay_ms(MAIN_LOOP_DELAY);
		
		
		if (config.on)
		{
			if ((counter & 0x1F) == 0)
			{
				PORTB ^= (1<<5);	// "L" LED on arduino nano clone	
			}
		}
		else
		{
			if (counter < 5)
			{
				PORTB |= (1<<5);
			}
			else
			{
				PORTB &= ~(1<<5);
			}
		}
		
		if (++counter == 200)
		{
			counter = 0;
			if (config.on)
			{
				RDA5807M_get_status(&status);
				LOG(("status: frequency %u.%02uMHz, stereo %u, rssi %u, tuneok %u, fmtrue %u, fmready %u, tunefail %u\n",
					status.frequency/100, status.frequency%100,
					status.stereo, status.rssi, status.tuneok, status.fmtrue, status.fmready, status.tunefail
					));
			}
			if (status.valid && config.frequency != status.frequency)
			{
				config_changed = 1;
			}
			if (config_changed)
			{
				// limiting EEPROM write frequency
				config.frequency = status.frequency;
				config_changed = 0;
				store_config();
			}
		}

		ui_state_poll(&ui_state);
	}
}
