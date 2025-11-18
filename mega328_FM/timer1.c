#include "timer1.h"
#include <avr/io.h>

enum { PERIOD = 100 };

void start_timer(void)
{
	TCCR1B=1;  							//taktowanie timera bez preskalera
	TCNT1=0;								//wyzerowanie timera
	OCR1A=10;								//"wstepna" wartosc rejestru output compare A
	OCR1B=PERIOD;						
	TIFR1&=~((1<<TOV1)|(1<<OCF1A)|(1<<OCF1B));			//skasowanie ewentualnych flag przerwan
	TIMSK1|=((1<<OCIE1A)|(1<<OCIE1B));  	//start timera - odblokowanie wykorzystywanych przerwan przepelnienia i output compare A
}

void stop_timer(void)
{
	TIMSK1&=~((1<<TOIE1)|(1<<OCIE1A)|(1<<OCIE1B));  		//deaktywacja przerwan
	TIFR1&=~((1<<TOV1)|(1<<OCF1A)|(1<<OCF1B));			//skasowanie ewentualnych flag przerwan
	TCCR1B=0;  								//zatrzymanie taktowania
}

void set_timer_duty_cycle(uint8_t percent)
{
	if (percent > 0 && percent < 100)
		OCR1A = (uint32_t)PERIOD*percent/100;
}
