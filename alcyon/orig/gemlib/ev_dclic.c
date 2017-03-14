#include "aeslib.h"

int16_t evnt_dclick(P(int16_t) rate, P(int16_t) setit)
PP(int16_t rate;)
PP(int16_t setit;)
{
	EV_DCRATE = rate;
	EV_DCSETIT = setit;
	return crys_if(EVNT_DCLICK);
}
