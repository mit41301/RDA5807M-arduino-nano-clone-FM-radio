#ifndef COMMANDS_H
#define COMMANDS_H

enum Command
{
	CMD_ON_OFF = 0,
	CMD_TUNE_UP,
	CMD_TUNE_DOWN,
	CMD_VOL_UP,
	CMD_VOL_DOWN,
	CMD_STEREO_TOGGLE,
	CMD_CHANNEL_MEM_SET,
	CMD_SLEEP_TIMER,
	
	CMD_0,
	CMD_1,
	CMD_2,
	CMD_3,
	CMD_4,
	CMD_5,
	CMD_6,
	CMD_7,
	CMD_8,
	CMD_9,

	CMD_CH_MINUS,
	CMD_CH_PLUS,
	
	CMD__LIMITER
};

const char* command_get_name(enum Command cmd);

#endif