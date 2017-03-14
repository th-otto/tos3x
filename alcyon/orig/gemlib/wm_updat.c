#include "aeslib.h"

int16_t wind_update(P(int16_t) beg_update)
PP(int16_t beg_update;)
{
	WM_BEGUP = beg_update;
	return crys_if(WIND_UPDATE);
}
