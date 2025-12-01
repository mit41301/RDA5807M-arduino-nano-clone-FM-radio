#include "ir_remote_s1_car_mp3.h"

/** \brief IR codes for ebay $1 mini "Car mp3" remote controller with 21 keys
*/

#define IR_ADDRESS 0x00

// buttons: left to right, top to bottom

#define IR_CH_MINUS 	0x45
#define IR_CH			0x46
#define IR_CH_PLUS		0x47
#define IR_PREV			0x44
#define IR_NEXT			0x40
#define IR_PLAY_PAUSE	0x43
#define IR_VOL_MINUS	0x07
#define IR_VOL_PLUS		0x15
#define IR_EQ			0x09
#define IR_0			0x16
#define IR_100			0x19
#define IR_200			0x0D
#define IR_1			0x0C
#define IR_2			0x18
#define IR_3			0x5E
#define IR_4			0x08
#define IR_5			0x1C
#define IR_6			0x5A
#define IR_7			0x42
#define IR_8			0x52
#define IR_9			0x4A


enum Command ir_translate(uint8_t address, uint8_t code)
{
	enum Command ret = CMD__LIMITER;
	if (address != 0x00)
	{
		return ret;
	}
	switch (code)
	{
	case IR_PLAY_PAUSE:
		return CMD_ON_OFF;
	case IR_NEXT:
		return CMD_TUNE_UP;
	case IR_PREV:
		return CMD_TUNE_DOWN;
	case IR_VOL_PLUS:
		return CMD_VOL_UP;
	case IR_VOL_MINUS:
		return CMD_VOL_DOWN;
	case IR_EQ:
		return CMD_STEREO_TOGGLE;
	}
	return ret;
}
