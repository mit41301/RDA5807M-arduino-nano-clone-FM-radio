/** FM radio based on RDA5807M module (native mode).
	Built with WinAVR 20100110
	
	Internal pull-ups are used with SDA (PC4) and SCL (PC5).
    As RDA5807M module operates with 3.3V logic, 3.3kOhm resistors are used in series with SDA and SCL
    for additional protection (if e.g. SDA/SCL would be accidentally configured in push-pull mode
    instead of weak internal pull-ups).
	
	IR receiver (VS1838B or similar) output is connected to PD2 / INT0 (see libnecdecoder.c).
*/
#include "RDA5807M.h"
#include "uart.h"
#include "radio_config.h"
#include "hw_config.h"
#include "timer1.h"
#include "libnecdecoder.h"
#include "ir_remote_s1_car_mp3.h"
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

static void init(void)    
{
	DDRB = 0XFF;
	// /** \note set minimum volume at startup */
	PORTB = 0x00;
	
	DDRC = 0B00000000;
	PORTC = 0Xff;	// internal pull-ups

	//_delay_ms(100);
	

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

	if (config.on)
	{
		RDA5807M_start();
	}
	else
	{
		RDA5807M_shutdown();
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

		// auto search
		if (cmd == CMD_ON_OFF)
		{
			if (config.on)
			{
				config.on = 0;
				RDA5807M_get_status(&status);
				RDA5807M_shutdown();
			}
			else
			{
				config.on = 1;
				RDA5807M_start();
			}			
			config_changed = 1;
		}
		else if (cmd == CMD_TUNE_DOWN)
		{
			RDA5807M_search(0);
			searching = 1;
			_delay_ms(500);
		}
		else if (cmd == CMD_TUNE_UP)
		{
			RDA5807M_search(1);		
			searching = 2;
			_delay_ms(500);
		}
		else if (cmd == CMD_VOL_UP)
		{
			if (config.volume < CONFIG_MAX_VOLUME)
			{
				config.volume++;
				RDA5807M_set_volume(config.volume);
				config_changed = 1;
			}
		}
		else if (cmd == CMD_VOL_DOWN)
		{
			if (config.volume > CONFIG_MIN_VOLUME)
			{
				config.volume--;
				RDA5807M_set_volume(config.volume);
				config_changed = 1;
			}			
		}
		else if (cmd == CMD_STEREO_TOGGLE)
		{
			config.stereo = !config.stereo;
			RDA5807M_set_stereo(config.stereo);
			config_changed = 1;
		}
		
		_delay_ms(20);
		
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
	}
}
