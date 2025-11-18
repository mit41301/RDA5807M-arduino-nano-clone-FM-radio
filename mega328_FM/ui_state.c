#include "ui_state.h"
#include "main.h"
#include <stdio.h>

#define LOCAL_DEBUG
#ifdef LOCAL_DEBUG
#	define LOG(args) (printf("UI_STATE "), printf args)
#else
#	define LOG(args)
#endif

enum
{
	// time after which UI state goes to idle if nothing happens in multi-key sequence
	STATE_TIMER = 3000/MAIN_LOOP_DELAY
};

void ui_state_reset(struct ui_state_s *s)
{
	s->st = UI_IDLE;
	s->timer = 0;
}

void ui_set_state(struct ui_state_s *s, enum ui_state_e st)
{
	s->st = st;
	if (st == UI_IDLE)
	{
		ui_state_reset(s);
	}
	else
	{
		s->timer = STATE_TIMER;
	}
}


void ui_state_poll(struct ui_state_s *s)
{
	if (s->timer > 0)
	{
		s->timer--;
		if (s->timer == 0)
		{
			LOG(("Resetting state (timeout)\n"));
			ui_state_reset(s);
		}
	}
}

