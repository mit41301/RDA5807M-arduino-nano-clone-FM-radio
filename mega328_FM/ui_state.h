#ifndef UI_STATE_H
#define UI_STATE_H

#include <stdint.h>

enum ui_state_e
{
	UI_IDLE = 0,
	
	UI_CHANNEL_SET_1,	// after CMD_CHANNEL_MEM_SET, waiting for memory number selection
	UI_CHANNEL_SET_2,	// after memory number selection, waiting for second CMD_CHANNEL_MEM_SET for confirmation
	
	UI_SLEEP,			// waiting for sleep time selection
};

struct ui_state_s
{
	enum ui_state_e st;
	int16_t timer;
	uint8_t channel_mem_id;
};


void ui_state_reset(struct ui_state_s *s);

void ui_set_state(struct ui_state_s *s, enum ui_state_e st);

void ui_state_poll(struct ui_state_s *s);


#endif
