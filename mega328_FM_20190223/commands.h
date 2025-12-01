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
	
	CMD__LIMITER
};

const char* command_get_name(enum Command cmd);

#endif