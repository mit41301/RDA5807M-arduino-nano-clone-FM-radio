// #############################################################################
// #              --- Infrared Remote Decoder (NEC Protocol) ---               #
// #############################################################################
// # libnecdecoder.c - NEC IR Library                                          #
// #############################################################################
// #              Version: 1.1 - Compiler: AVR-GCC 4.5.3 (Linux)               #
// #      (c) 2013 by Malte Pöggel - All rights reserved. - License: BSD       #
// #               www.MALTEPOEGGEL.de - malte@maltepoeggel.de                 #
// #############################################################################
// #   Redistribution and use in source and binary forms, with or without mo-  #
// # dification, are permitted provided that the following conditions are met: #
// #                                                                           #
// # * Redistributions of source code must retain the above copyright notice,  #
// #   this list of conditions and the following disclaimer.                   #
// # * Redistributions in binary form must reproduce the above copyright       #
// #   notice, this list of conditions and the following disclaimer in the     #
// #   documentation and/or other materials provided with the distribution.    #
// #                                                                           #
// #    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS    #
// # "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED #
// #      TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A      #
// #     PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT    #
// #   HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,  #
// # SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED  #
// #    TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,    #
// #  OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY   #
// #  OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING  #
// #    NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS     #
// #      SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.         #
// #############################################################################

#include "libnecdecoder.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>

#define LOCAL_DEBUG
#ifdef LOCAL_DEBUG
#	define LOG(args) (printf("NEC "), printf args)
#else
#	define LOG(args)
#endif

//#define LOCAL_DEBUG_HEAVY
#ifdef LOCAL_DEBUG_HEAVY
#	define LOG_HEAVY(args) (printf args)
#else
#	define LOG_HEAVY(args)
#endif


/*
	IR receiver output connected to PD2 / INT0
*/

/*

REGULAR CODE:
- 9ms leading pulse burst (16 times the pulse burst length used for a logical data bit)
- 4.5ms space
- 8-bit address for the receiving device
- 8-bit logical inverse of the address
- 8-bit command
- 8-bit logical inverse of the command
- final 562.5µs pulse burst to signify the end of message transmission

REPEAT CODE:
- 9ms leading pulse burst
- 2.25ms space
- 562.5µs pulse burst to mark the end of the space (and hence end of the transmitted repeat code)

*/

#if defined (__AVR_ATmega328P__) || defined (__AVR_ATmega328__)	
// arduino nano: 16MHz crystal
#define CLOCK_SCALE 2
#else
// default: 8MHz clock
#define CLOCK_SCALE 1
#endif

// AGC Burst, 9ms typ
#define TIME_BURST_MIN (65*CLOCK_SCALE)
#define TIME_BURST_MAX (85*CLOCK_SCALE)	// my $1 remote: 10 ms

// Gap after AGC Burst, 4.5ms typ
#define TIME_GAP_MIN   (30*CLOCK_SCALE)
#define TIME_GAP_MAX   (40*CLOCK_SCALE)

// Gap (key hold) after AGC Burst, 2.25ms typ
#define TIME_HOLD_MIN   (15*CLOCK_SCALE)
#define TIME_HOLD_MAX   (20*CLOCK_SCALE)

// Short pulse for each bit, 560us typ
#define TIME_PULSE_MIN  (1*CLOCK_SCALE)
#define TIME_PULSE_MAX  (8*CLOCK_SCALE)

// Gap for logical 0, 560us typ
#define TIME_ZERO_MIN   (1*CLOCK_SCALE)
#define TIME_ZERO_MAX   (7*CLOCK_SCALE)

// Gap for logical 1, 1.69ms typ
#define TIME_ONE_MIN    (9*CLOCK_SCALE)
#define TIME_ONE_MAX   (19*CLOCK_SCALE)


// Timer Overflows till keyhold flag is cleared
#define IR_HOLD_OVF (5*CLOCK_SCALE)


// Definition for state machine
enum ir_state_t
{
	IR_IDLE = 0,
	IR_BURST,
	IR_GAP,
	IR_ADDRESS,
	IR_ADDRESS_INV,
	IR_COMMAND,
	IR_COMMAND_INV
};


volatile uint8_t ir_state;
volatile uint8_t ir_bitctr;
#ifdef PROTOCOL_NEC_EXTENDED
volatile uint8_t ir_tmp_address_l;
volatile uint8_t ir_tmp_address_h;
#else
volatile uint8_t ir_tmp_address;
#endif
volatile uint8_t ir_tmp_command;
volatile uint8_t ir_tmp_keyhold;
volatile uint8_t ir_tmp_ovf;


volatile struct ir_struct ir;


void ir_init( void ) {
	// activate pull-up on PORTD2
	PORTD |= (1 << PD2);

    // tAGC_burst = 9ms, tBIT = 0.56ms
#if defined (__AVR_ATmega328P__) || defined (__AVR_ATmega328__)	
    // Timer: 8bit, Clock: 16MHz, Prescaler 1024, Overflow = 32.768ms/2, Tick = 0.128ms/2
    TCCR0A &= ~( (1<<COM0A0) | (1<<COM0A1) | (1<<COM0B0) | (1<<COM0B1) | (1<<WGM00) | (1<<WGM01) );
    TCCR0B |= (1<<CS00) | (1<<CS02);
    TIMSK0 |= (1<<TOIE0);

    // Interrupt 0 (PD2): Inverted signal input, triggered by (any) logical change
    DDRD   &= ~(1<<PD2);
    EICRA  |= (1<<ISC00);
    EIMSK  |= (1<<INT0);
#elif defined (__AVR_ATmega48__) || defined(__AVR_ATmega88__) || defined(__AVR_ATmega168__)
    // Timer: 8bit, Clock: 8MHz, Prescaler 1024, Overflow = 32.768ms, Tick = 0.128ms
    TCCR0A &= ~( (1<<COM0A0) | (1<<COM0A1) | (1<<COM0B0) | (1<<COM0B1) | (1<<WGM00) | (1<<WGM01) );
    TCCR0B |= (1<<CS00) | (1<<CS02);
    TIMSK0 |= (1<<TOIE0);

    // Interrupt 0 (PD2): Inverted signal input, triggered by logical change
    DDRD   &= ~(1<<PD2);
    EICRA  |= (1<<ISC00);
    EIMSK  |= (1<<INT0);
#elif defined (__AVR_ATtiny2313__) || (__AVR_ATtiny4313__)
    // Timer: 8bit, Clock: 8MHz, Prescaler 1024, Overflow = 32.768ms, Tick = 0.128ms
    TCCR0A &= ~( (1<<COM0A0) | (1<<COM0A1) | (1<<COM0B0) | (1<<COM0B1) | (1<<WGM00) | (1<<WGM01) );
    TCCR0B |= (1<<CS00) | (1<<CS02);
    TIMSK  |= (1<<TOIE0);

    // Interrupt 0 (PD2): Inverted signal input, triggered by logical change
    DDRD   &= ~(1<<PD2);
    MCUCR  |= (1<<ISC00);
    GIMSK  |= (1<<INT0);
#else
#error "MCU not supported"
#endif

    // Reset state
    ir_state = IR_IDLE;

    // Reset global variables
    ir_tmp_keyhold = 0;
    ir_tmp_ovf = 0;
}


void ir_stop( void ) {
    // Stop timer and disable interrupt
#if defined (__AVR_ATmega328P__) || defined (__AVR_ATmega328__)		
    TCCR0B &= ~((1<<CS00) | (1<<CS02));
    TIMSK0 &= ~(1<<TOIE0);
    EIMSK  &= ~(1<<INT0);
#elif defined (__AVR_ATmega48__) || defined(__AVR_ATmega88__) || defined(__AVR_ATmega168__)
    TCCR0B &= ~((1<<CS00) | (1<<CS02));
    TIMSK0 &= ~(1<<TOIE0);
    EIMSK  &= ~(1<<INT0);
#elif defined (__AVR_ATtiny2313__) || (__AVR_ATtiny4313__)
    TCCR0B &= ~((1<<CS00) | (1<<CS02));
    TIMSK  &= ~(1<<TOIE0);
    GIMSK  &= ~(1<<INT0);
#else
#error "MCU not supported"
#endif
}


// ###### INT0 for decoding ######
// there would be 72 interrupts for each full transmission
ISR( INT0_vect ) {
    // Get current port state to check if we triggered on rising or falling edge
    uint8_t port_state = ( PIND & (1<<PD2) );
    uint8_t cnt_state = TCNT0;
	
	//printf(".");
#ifdef LOCAL_DEBUG_HEAVY
	if (port_state) {
		putchar('1');
	} else {
		putchar('0');
	}
#endif

    if (ir_tmp_ovf) {
        // timer overflow - too much time passed since last state change
        ir_tmp_ovf = 0;
        TCNT0 = 0;		
		if (port_state) {
			LOG_HEAVY(("\n#I"));		
			ir_state = IR_IDLE;
		} else {
			LOG_HEAVY(("\n#B"));		
			ir_state = IR_BURST;
		}
        return;
    }

    switch(ir_state) {
	case IR_IDLE:
		if (!port_state) {
			ir_state = IR_BURST;
			TCNT0 = 0;
		}
		break;
    case IR_BURST:
        if(port_state) {
            if( (cnt_state>TIME_BURST_MIN) && (cnt_state<TIME_BURST_MAX) ) {
                //LOG(("IR_GAP\n"));
				LOG_HEAVY(("G %u\n", cnt_state));
				ir_state = IR_GAP; // Next state
                TCNT0 = 0; // Reset counter
            } else {
				LOG_HEAVY(("I2 %u\n", cnt_state));
				ir_state = IR_IDLE;
                TCNT0 = 0;			
			}
        }
        break;
    case IR_GAP:
        if (!port_state) {
            if ( (cnt_state>TIME_GAP_MIN) && (cnt_state<TIME_GAP_MAX) ) {
                TCNT0 = 0; // Reset counter
				LOG_HEAVY(("A"));
                ir_state = IR_ADDRESS; // Next state
                ir_bitctr = 0; // Reset bitcounter
                ir.status &= ~(1<<IR_KEYHOLD);
                break;
            } else if ( (cnt_state>TIME_HOLD_MIN) && (cnt_state<TIME_HOLD_MAX) ) {
                if(ir.status & (1<<IR_SIGVALID)) {
                    ir.status |= (1<<IR_KEYHOLD);
                    ir_tmp_keyhold = IR_HOLD_OVF;
                }
				LOG(("IDLE 2 cnt = %d\n", cnt_state));
                ir_state = IR_IDLE;
                break;
            }
        }
        // Should not happen, must be invalid. Reset.
		LOG(("IDLE 3 port %d, cnt %d\n", port_state, cnt_state));
        ir_state = IR_IDLE;
        break;
    case IR_ADDRESS:
        if(port_state) {
            // Must be short pulse
            if((cnt_state>TIME_PULSE_MIN)&&(cnt_state<TIME_PULSE_MAX)) {
                TCNT0 = 0; // Reset counter
                break;
            }
            // Should not happen, must be invalid. Reset.
			LOG(("IDLE 4 cnt %u\n", cnt_state));			
            ir_state = IR_IDLE;
        } else {
            if((cnt_state>TIME_ZERO_MIN)&&(cnt_state<TIME_ZERO_MAX)) {
                // 0
#ifdef PROTOCOL_NEC_EXTENDED
                ir_tmp_address_l &= ~(1<<ir_bitctr++);
#else
                ir_tmp_address &= ~(1<<ir_bitctr++);
#endif
                TCNT0 = 0; // Reset counter
                if(ir_bitctr>=8) {
					LOG_HEAVY(("AI\n"));
                    ir_state = IR_ADDRESS_INV; // Next state
                    ir_bitctr = 0; // Reset bitcounter
                }
                break;
            } else if((cnt_state>TIME_ONE_MIN)&&(cnt_state<TIME_ONE_MAX)) {
                // 1
#ifdef PROTOCOL_NEC_EXTENDED
                ir_tmp_address_l |= (1<<ir_bitctr++);
#else
                ir_tmp_address |= (1<<ir_bitctr++);
#endif
                TCNT0 = 0; // Reset counter
                if(ir_bitctr>=8) {
					LOG(("IR_ADDR inv\n"));
                    ir_state = IR_ADDRESS_INV; // Next state
                    ir_bitctr = 0; // Reset bitcounter
                }
                break;
            }
            // Should not happen, must be invalid. Reset.
			LOG(("IDLE 5 cnt %u\n", cnt_state));				
            ir_state = IR_IDLE;
            break;
        }
        break;
    case IR_ADDRESS_INV:
        if(port_state) {
            // Must be short pulse
            if((cnt_state>TIME_PULSE_MIN)&&(cnt_state<TIME_PULSE_MAX)) {
                TCNT0 = 0; // Reset counter
                break;
            }
            // Should not happen, must be invalid. Reset.
            ir_state = IR_IDLE;
        } else {
            if((cnt_state>TIME_ZERO_MIN)&&(cnt_state<TIME_ZERO_MAX)) {
                // 0 (inverted) or high address
#ifdef PROTOCOL_NEC_EXTENDED
                ir_tmp_address_h &= ~(1<<ir_bitctr++);
#else
                if(!(ir_tmp_address&(1<<ir_bitctr++))) {
                    // Should not happen, must be invalid. Reset.
                    ir_state = IR_IDLE;
                    break;
                }
#endif
                TCNT0 = 0; // Reset counter
                if(ir_bitctr>=8) {
				#ifdef LOCAL_DEBUG_HEAVY
					putchar('C');
				#endif
                    ir_state = IR_COMMAND; // Next state
                    ir_bitctr = 0; // Reset bitcounter
                }
                break;
            } else if((cnt_state>TIME_ONE_MIN)&&(cnt_state<TIME_ONE_MAX)) {
                // 1 (inverted) or high address
#ifdef PROTOCOL_NEC_EXTENDED
                ir_tmp_address_h |= (1<<ir_bitctr++);
#else
                if(ir_tmp_address&(1<<ir_bitctr++)) {
                    // Should not happen, must be invalid. Reset.
                    ir_state = IR_IDLE;
                    break;
                }
#endif
                TCNT0 = 0; // Reset counter
                if(ir_bitctr>=8) {
				#ifdef LOCAL_DEBUG_HEAVY
					putchar('C');
				#endif
                    ir_state = IR_COMMAND; // Next state
                    ir_bitctr = 0; // Reset bitcounter
                }
                break;
            }
            // Should not happen, must be invalid. Reset.
            ir_state = IR_IDLE;
            break;
        }
        break;
    case IR_COMMAND:
        if(port_state) {
            // Must be short pulse
            if((cnt_state>TIME_PULSE_MIN)&&(cnt_state<TIME_PULSE_MAX)) {
                TCNT0 = 0; // Reset counter
                break;
            }
            // Should not happen, must be invalid. Reset.
            ir_state = IR_IDLE;
        } else {
            if((cnt_state>TIME_ZERO_MIN)&&(cnt_state<TIME_ZERO_MAX)) {
                // 0
                ir_tmp_command &= ~(1<<ir_bitctr++);
                TCNT0 = 0; // Reset counter
                if(ir_bitctr>=8) {
					LOG_HEAVY(("CI"));
                    ir_state = IR_COMMAND_INV; // Next state
                    ir_bitctr = 0; // Reset bitcounter
                }
                break;
            } else if((cnt_state>TIME_ONE_MIN)&&(cnt_state<TIME_ONE_MAX)) {
                // 1
                ir_tmp_command |= (1<<ir_bitctr++);
                TCNT0 = 0; // Reset counter
                if(ir_bitctr>=8) {
					LOG_HEAVY(("CI"));				
                    ir_state = IR_COMMAND_INV; // Next state
                    ir_bitctr = 0; // Reset bitcounter
                }
                break;
            }
            // Should not happen, must be invalid. Reset.
            ir_state = IR_IDLE;
            break;
        }
        break;
    case IR_COMMAND_INV:
        if(port_state) {
            // Must be short pulse
            if((cnt_state>TIME_PULSE_MIN)&&(cnt_state<TIME_PULSE_MAX)) {
                TCNT0 = 0; // Reset counter
                break;
            }
            // Should not happen, must be invalid. Reset.
            ir_state = IR_IDLE;
        } else {
            if((cnt_state>TIME_ZERO_MIN)&&(cnt_state<TIME_ZERO_MAX)) {
                // 0 (inverted)
                if(!(ir_tmp_command&(1<<ir_bitctr++))) {
                    // Should not happen, must be invalid. Reset.
                    ir_state = IR_IDLE;
                    break;
                }
                TCNT0 = 0; // Reset counter
                if(ir_bitctr>=8) {
                    ir_state = IR_IDLE; // Decoding finished.
                    // Only apply if received flag is not set, must be done
                    // by the main program after reading address and command
                    if(!(ir.status & (1<<IR_RECEIVED))) {
#ifdef PROTOCOL_NEC_EXTENDED
                        ir.address_l = ir_tmp_address_l;
                        ir.address_h = ir_tmp_address_h;
#else
                        ir.address = ir_tmp_address;
#endif
                        ir.command = ir_tmp_command;
                        ir.status |= (1<<IR_RECEIVED) | (1<<IR_SIGVALID);
						LOG(("Received addr %02X cmd %02X\n", ir.address, ir.command));
                        ir_tmp_keyhold = IR_HOLD_OVF; // To make shure that valid flag is cleared
                    } else {
						LOG(("Received addr %02X cmd %02X but RCV flag is not cleared, event dropped\n", ir_tmp_address, ir_tmp_command));					
					}
                    ir_bitctr = 0; // Reset bitcounter
                }
                break;
            } else if((cnt_state>TIME_ONE_MIN)&&(cnt_state<TIME_ONE_MAX)) {
                // 1 (inverted)
                if(ir_tmp_command&(1<<ir_bitctr++)) {
                    // Should not happen, must be invalid. Reset.
                    ir_state = IR_IDLE;
                    break;
                }
                TCNT0 = 0; // Reset counter
                if(ir_bitctr>=8) {
                    ir_state = IR_IDLE; // Decoding finished.
                    // Only apply if received flag is not set, must be done
                    // by the main program after reading address and command
                    if(!(ir.status & (1<<IR_RECEIVED))) {
#ifdef PROTOCOL_NEC_EXTENDED
                        ir.address_l = ir_tmp_address_l;
                        ir.address_h = ir_tmp_address_h;
#else
                        ir.address = ir_tmp_address;
#endif
                        ir.command = ir_tmp_command;
                        ir.status |= (1<<IR_RECEIVED) | (1<<IR_SIGVALID);
						LOG(("Received addr %02X cmd %02X\n", ir.address, ir.command));						
                        ir_tmp_keyhold = IR_HOLD_OVF; // To make shure that valid flag is cleared
                    } else {
						LOG(("Received addr %02X cmd %02X but RCV flag is not cleared\n", ir_tmp_address, ir_tmp_command));					
					}
                    ir_bitctr = 0; // Reset bitcounter
                }
                break;
            }
            // Should not happen, must be invalid. Reset.
            ir_state = IR_IDLE;
			LOG(("IDLE 6 cnt %u bits %d\n", cnt_state, ir_bitctr));			
            break;
        }
        break;
    }
}


// ###### Timer 0 Overflow for hold flag clear ######
ISR (TIMER0_OVF_vect) {
	if (!ir_tmp_ovf) {
	#ifdef LOCAL_DEBUG_HEAVY		
		putchar('?');
	#endif
		ir_tmp_ovf = 1;
	}
    if(ir_tmp_keyhold>0) {
        ir_tmp_keyhold--;
        if(ir_tmp_keyhold==0) ir.status &= ~((1<<IR_KEYHOLD) | (1<<IR_SIGVALID));
    }
}
