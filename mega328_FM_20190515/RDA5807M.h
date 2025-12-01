#ifndef RDA5807M_H
#define RDA5807M_H

#include <stdint.h>
#include <stdbool.h>

uint8_t RDA5807M_init(void);

void RDA5807M_search(uint8_t up);

/** \param value Tuned frequency, unit = 10kHz */
void RDA5807M_tune(uint16_t value);

void RDA5807M_set_volume(uint8_t volume);

void RDA5807M_set_stereo(uint8_t on);

struct RDA5807M_status
{
	uint8_t valid;
	uint16_t frequency;	// unit = 10kHz
    uint8_t stereo;
    uint8_t rssi;
    uint8_t tuneok;
    uint8_t fmtrue;
    uint8_t fmready;
    uint8_t tunefail;
};

uint8_t RDA5807M_get_status(struct RDA5807M_status *status);

void RDA5807M_mute(bool state);

void RDA5807M_shutdown(void);

#endif
