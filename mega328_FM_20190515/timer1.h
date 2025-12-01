//obsluga timera 1
#ifndef TIMER1_H
#define TIMER1_H

#include <stdint.h>

void start_timer(void);

void stop_timer(void);

void set_timer_duty_cycle(uint8_t percent);

#endif

