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
	case CMD_CHANNEL_MEM_SET:
		return "CHANNEL MEM SET";
	case CMD_SLEEP_TIMER:
		return "SLEEP TIMER";
	
	case CMD_0:
		return "[0]";
	case CMD_1:
		return "[1]";
	case CMD_2:
		return "[2]";
	case CMD_3:
		return "[3]";
	case CMD_4:
		return "[4]";
	case CMD_5:
		return "[5]";
	case CMD_6:
		return "[6]";
	case CMD_7:
		return "[7]";
	case CMD_8:
		return "[8]";
	case CMD_9:
		return "[9]";
		
	case CMD_CH_MINUS:
		return "CH_MINUS";
	case CMD_CH_PLUS:
		return "CH_PLUS";
	
	default:
		return "???";
	}
}