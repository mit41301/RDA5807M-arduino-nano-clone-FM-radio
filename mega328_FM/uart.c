#include "uart.h"
#include <avr/io.h>
#include <stdio.h>


int uart_putchar (char c, FILE *f)
{
    while(! (UCSR0A & (1 << UDRE0))){}
    UDR0 = c;
	return 0;
}

void UART_Init (void)
{
//	UCSRB=(1 << TXEN);
//	UBRRL=(F_CPU / (BAUD_RATE * 16L) - 1);

	#define BAUD 115200
	#include <util/setbaud.h> 
	UBRR0H = UBRRH_VALUE;
	UBRR0L = UBRRL_VALUE;

#if USE_2X
	UCSR0A |= (1 << U2X0);
#else
	UCSR0A &= ~(1 << U2X0);
#endif
	/* Enable USART transmitter/receiver */
	UCSR0B = (1 << TXEN0) | (1 << RXEN0);
	UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);   /* 8 data bits, 1 stop bit */	
	
	fdevopen (uart_putchar, 0);
}

