#include "commands.h"

const char* command_get_name(enum Command cmd)
{
	switch (cmd)
	{
	case CMD_ON_OFF:
		return "ON/OFF";
	case CMD_TUNE_UP:
		return "TUNE UP";
	case CMD_TUNE_DOWN:
		return "TUNE DOWN";
	case CMD_VOL_UP:
		return "VOL UP";
	case CMD_VOL_DOWN:
		return "VOL DOWN";
	case CMD_STEREO_TOGGLE:
		return "STEREO TOGGLE";
	
	default:
		return "???";
	}
}