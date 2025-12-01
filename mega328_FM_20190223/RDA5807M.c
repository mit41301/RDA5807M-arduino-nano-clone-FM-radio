#include "RDA5807M.h"
#include "uart.h"
#include "avr_twi.h"
#include <avr/io.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>

#define LOCAL_DEBUG
#ifdef LOCAL_DEBUG
#define LOG(args) (printf("RDA5807 "), printf args)
#endif

/** \note Do not mistake with 7-bit adressing style
*/
#define SLA_W (0b00100000)		///< I2C write address
#define SLA_R (SLA_W | 0x01)	///< I2C read address

#define SLA_SINGLE_W 0x22


#define R2                  0
#define R3                  1
#define R4                  2
#define R5                  3
#define R6                  4
#define R7                  5    
    
#define R2_DHIZ             0x8000	///< Audio Output High-Z Disable; 0= high impedance, 1= normal operation; default = 0
#define R2_DMUTE            0x4000	///< Mute disable (1 = normal operation); default = 0;
#define R2_MONO             0x2000	///< 1 = force mono; default = 0
#define R2_BASS             0x1000	///< 1 = bass boost; default = 0
#define R2_SEEKUP           0x0200	///< seek direction; 1 = up; default = 0
#define R2_SEEK             0x0100	///< start seek
#define R2_SKMODE           0x0080	///< seek mode; 0 = wrap at band limit, 1 = stop and band limit; default = 0
#define R2_RDS_EN           0x0008	///< enable RDS/RBDS; default = 0
#define R2_CTRL_NEW         0x0004	///< "new demodulate method enable, can improve the sensitivity by 1dB"
#define R2_SOFT_RESET       0x0002	///< default = 0
#define R2_ENABLE           0x0001	///< power up enable; default = 0


// R3 bits 15:6 (10 bits) = channel select; frequency = CHAN * channel select + BAND_START
#define R3_CHAN_MASK		0xFFC0
#define R3_CHAN_OFFSET		6
// bit 5 = direct mode, "only used when test"
#define R3_TUNE             0x0010	///< The tune operation begins when the TUNE bit is set high. The STC bit is set high when the tune operation completes.
									///< The tune bit is reset to low automatically when the tune operation completes.
// bits 3:2 = band select
#define R3_BAND_SEL_87_108  0x0000	//(US/Europe)
#define R3_BAND_SEL_76_91   0x0004	//(Japan)
#define R3_BAND_SEL_76_108  0x0008  //(world wide)
#define R3_BAND_SEL_OTHER   0x000C	// 65-76 MHz East Europe or 50-65MHz depending on R7_65M_50M_MODE
// bits 1:0 = channel select
#define R3_CH_SPACE_100K    0x0000
#define R3_CH_SPACE_200K    0x0001
#define R3_CH_SPACE_50K     0x0002
#define R3_CH_SPACE_25K     0x0003									


#define R4_DE               0x0800
#define R4_SOFTMUTE_EN      0x0200
#define R4_AFCD             0x0100

#define R5_INT_MODE         0x8000
#define R5_UNDOCUMENTED_SHIT 0x0080 // "Resvered" in datasheet 1.1, default = 0, but reception is very poor on my module from 2017 if not set
// bits 3:0 = volume control, 0000 = mute, 1111 = max
#define R5_VOLUME_MASK		0x000F
#define R5_VOLUME_OFFSET	0

#define R7_65M_50M_MODE     0x0200
#define R7_SOFTBLEND_EN     0x0002
#define R7_FREQ_MODE        0x0001


struct REG
{
	unsigned char reg_addr;        
	unsigned short value;
} regs[6];


static void RDA5807M_write_reg(uint8_t regidx)
{
    LOG(("write_reg %u\n", regidx));
	twi_start();
	twi_write_address(SLA_SINGLE_W);	// address
	twi_write_data(regs[regidx].reg_addr);
    twi_write_data(regs[regidx].value >> 8);
	twi_write_data(regs[regidx].value & 0xFF);
	twi_stop();
	LOG(("stopped\n"));
}

uint8_t RDA5807M_write(void)
{
	uint8_t ret = 0;
	LOG(("write\n"));
	ret = twi_start();
	if (ret)
	{
		return ret;
	}
	
	// send address
	LOG(("send address\n"));
	ret = twi_write_address(SLA_W);
	if (ret)
	{
		LOG(("Write error @address\n"));
		return ret;
	}
	
	uint8_t twst = TW_STATUS & 0xF8;
	if ( (twst != TW_MT_SLA_ACK) && (twst != TW_MR_SLA_ACK) )
	{
		LOG(("Write error @ADDR, twst = 0x%02X\n", twst));
		ret = 1;	
	}
	
	LOG(("send registers\n"));
	for (uint8_t i = 0; i < sizeof(regs)/sizeof(regs[0]); i++)			// send registers
	{
uint16_t value = regs[i].value;
		ret = twi_write_data(value >> 8);
		if (ret)
			break;

		ret = twi_write_data(value & 0xFF);
		if (ret)
			break;


	}
	
	LOG(("stop\n"));
	TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWSTO);	// I2C stop
	// wait until stop condition is executed and bus released
	while(TWCR & (1<<TWSTO));
	LOG(("stopped\n"));
	return ret;	
}


uint8_t RDA5807M_init(void)
{
	LOG(("init\n"));
	
	regs[R2].reg_addr = 0x02;
	regs[R2].value = R2_DHIZ | R2_DMUTE | R2_MONO | R2_CTRL_NEW | R2_ENABLE /* | R2_RDS_EN */;

	regs[R3].reg_addr = 0x03;
	regs[R3].value = R3_BAND_SEL_87_108 | R3_CH_SPACE_100K;

	regs[R4].reg_addr = 0x04;
	regs[R4].value = 0;

	regs[R5].reg_addr = 0x05;
	/* Warning: undocumented shit below - if 0x0080 is skipped (in DS 1.1 "Resvered") reception is very poor
	 */
	regs[R5].value = R5_INT_MODE | 0x0800 /* seek threshold */ | R5_UNDOCUMENTED_SHIT | 0x0000 /* volume: muted */;

	regs[R6].reg_addr = 0x06;
	regs[R6].value = 0x0000;

	regs[R7].reg_addr = 0x07;
	regs[R7].value = 0x4000 | R7_65M_50M_MODE | R7_SOFTBLEND_EN;

	// reset
	regs[R2].value |= R2_SOFT_RESET;
	RDA5807M_write_reg(R2);	
	regs[R2].value &= ~R2_SOFT_RESET;
	
    return RDA5807M_write();
}


void RDA5807M_tune(uint16_t value)
{
    //RDA5807M_write(tune_config, TUNE_CONFIG_LEN);
	
	uint16_t chan = (value - 8700) / 10;
	LOG(("Tune to %u.%02uMHz, chan = %u\n", value/100, value%100, chan));
	
	uint16_t r3= regs[R3].value;
	r3 &= ~R3_CHAN_MASK;
	r3 |= chan << R3_CHAN_OFFSET;
	r3 |= R3_TUNE;

	regs[R3].value = r3;
    RDA5807M_write_reg(R3);
    //RDS_reset();	
}

void RDA5807M_search(uint8_t up)
{
	LOG(("search: up = %d\n", up));
	if (up)
	{
		regs[R2].value |= (R2_SEEKUP|R2_SEEK);
	}
	else
	{
		regs[R2].value &= ~R2_SEEKUP;
		regs[R2].value |= R2_SEEK;	
	}
	RDA5807M_write_reg(R2);
	regs[R2].value &= ~R2_SEEK;
    //RDS_reset();
}

uint8_t RDA5807M_get_status(struct RDA5807M_status *status)
{
	//LOG(("get_status\n"));
	status->valid = 0;
	
	uint8_t i = 0;
    uint8_t data[12] = {0};
	uint16_t buffer[6] = {0};

	uint8_t rdsready = 0;
	uint8_t rds_sync =0;
	uint8_t blke = 0;

	uint8_t ABCD_E = 0;
	uint8_t BLERA = 0;
    uint8_t BLERB = 0;
    
    uint8_t ret;
	
	ret = twi_start();
	if (ret)
	{
		return ret;
	}
	ret = twi_write_address(SLA_R);
	if (ret)
	{
		return ret;
	}

	//LOG(("reading...\n"));
    for(i = 0; i < 12; ++i)
    {
		if (i != 11)
			TWACK;
		else
			TWNACK;
		while (!(TWCR & (1<<TWINT)));
		data[i] = TWDR; 	
    }

    twi_stop();
	//LOG(("stopped\n"));

    uint8_t x = 0;
	for(i = 0; i < 12; ++i)
	{
		buffer[x] = data[i];
        i++;
		buffer[x] <<= 8;
		buffer[x] |= data[i];
		x++;
	}

    status->frequency = (buffer[0] & 0x03FF) * 10 + 8700;
	
    // 0 - 3F (RSSI scale is logarithmic)
    status->rssi    = buffer[1] >> 9;
    
    // Stereo Indicator. 0 = Mono; 1 = Stereo
	status->stereo  = (buffer[0] >> 10) & 0x01;
    // 1=ready, 0=not ready
	status->fmready = buffer[1] & 0x0008;
    // 1 = the current channel is a station
    // 0 = the current channel is not a station
	status->fmtrue  = (buffer[1] >> 8) &0x01;
    // Seek/Tune Complete.
    // 0 = Not complete 1 = Complete
    // The seek/tune complete flag is set when the seek or tune operation completes.
	status->tuneok  = (buffer[0] >> 14) & 0x01;
    // RDS ready 0 = No RDS/RBDS group ready(default) 1 = New RDS/RBDS group ready    
	rdsready = (buffer[0] >> 15) & 0x01;
    // Seek Fail.
    // 0 = Seek successful; 1 = Seek failure
    // The seek fail flag is set when the seek operation fails to find a channel 
    // with an RSSI level greater than SEEKTH[5:0].    
    status->tunefail = (buffer[0] >> 13) & 0x01;
    // RDS Synchronization 
    // 0 = RDS decoder not synchronized(default) 
    // 1 = RDS decoder synchronized Available only in RDS Verbose mode    
	rds_sync = (buffer[0] >> 12) & 0x01;
    // When RDS enable:
    // 1 = Block E has been found
    // 0 = no Block E has been found
	blke = (buffer[0] >> 11) & 0x01;
    // 1= the block id of register 0cH,0dH,0eH,0fH is E
    // 0= the block id of register 0cH, 0dH, 0eH,0fH is A, B, C, D
	ABCD_E = (buffer[1] >> 4) & 0x01;
    // Block Errors Level of RDS_DATA_0, and is always read as Errors Level of 
    // RDS BLOCK A (in RDS mode) or BLOCK E (in RBDS mode when ABCD_E flag is 1)
    // 00= 0 errors requiring correction
    // 01= 1~2 errors requiring correction
    // 10= 3~5 errors requiring correction
    // 11= 6+ errors or error in checkword, correction not possible.
    // Available only in RDS Verbose mode    
    BLERA = (buffer[1] >> 2) & 0x03;
    // Block Errors Level of RDS_DATA_1, and is always read as Errors Level of 
    // RDS BLOCK B (in RDS mode ) or E (in RBDS mode when ABCD_E flag is 1).
    // 00= 0 errors requiring correction
    // 01= 1...2 errors requiring correction
    // 10= 3...5 errors requiring correction
    // 11= 6+ errors or error in checkword, correction not possible.
    // Available only in RDS Verbose mode            
	BLERB = buffer[1] & 0x03;

	//LOG(("%04X, %04X, %02X, %u\r\n", buffer[1], buffer[1] >> 9, info->rssi, info->rssi));    	
	//LOG(("RDSS %d, BLK_E %d ABCD_E %d BLERA %d BLERB %d\r\n", rds_sync, blke, ABCD_E, BLERA, BLERB));

    if(BLERA > 0 || BLERB > 0) 
    {
		//LOG(("BLERA %d BLERB %d\r\n", BLERA, BLERB));
    }
	else
	{    
		//if(rdsready)
		//	RDS_parse(&buffer[2]);
	}
	status->valid = 1;
	return 0;
}

void RDA5807M_set_volume(uint8_t volume)
{
	LOG(("Set volume = %u\n", volume));
	regs[R5].value &= ~R5_VOLUME_MASK;
	regs[R5].value |= (volume & R5_VOLUME_MASK);
	RDA5807M_write_reg(R5);
}

void RDA5807M_set_stereo(uint8_t on)
{
	LOG(("Set stereo = %u\n", on));
	if (on == 0)
	{
		LOG(("Forcing mono\n"));
		regs[R2].value |= R2_MONO;
	}
	else
	{
		regs[R2].value &= ~R2_MONO;	
	}
	RDA5807M_write_reg(R2);
}

void RDA5807M_shutdown(void)
{
	LOG(("shutdown\n"));
	regs[R2].value &= ~R2_ENABLE;
	RDA5807M_write_reg(R2);
}
