#include "aeslib.h"

int16_t evnt_timer(P(int16_t) locnt, P(int16_t) hicnt)
PP(int16_t locnt;)
PP(int16_t hicnt;)
{
	T_LOCOUNT = locnt;
	T_HICOUNT = hicnt;
	return crys_if(EVNT_TIMER);
}
