#include "aeslib.h"

int16_t wind_find(P(int16_t) mx, P(int16_t) my)
PP(int16_t mx;)
PP(int16_t my;)
{
	WM_MX = mx;
	WM_MY = my;
	return crys_if(WIND_FIND);
}
