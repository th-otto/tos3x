#include "aeslib.h"

int16_t wind_open(P(int16_t) handle, P(int16_t) wx, P(int16_t) wy, P(int16_t) ww, P(int16_t) wh)
PP(int16_t handle;)
PP(int16_t wx;)
PP(int16_t wy;)
PP(int16_t ww;)
PP(int16_t wh;)
{
	WM_HANDLE = handle;
	WM_WX = wx;
	WM_WY = wy;
	WM_WW = ww;
	WM_WH = wh;
	return crys_if(WIND_OPEN);
}
