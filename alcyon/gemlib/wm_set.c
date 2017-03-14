#include "aeslib.h"

int16_t wind_set(P(int16_t) w_handle, P(int16_t) w_field, P(int16_t) w2, P(int16_t) w3, P(int16_t) w4, P(int16_t) w5)
PP(int16_t w_handle;)
PP(int16_t w_field;)

PP(int16_t w2;)
PP(int16_t w3;)
PP(int16_t w4;)
PP(int16_t w5;)
{
	WM_HANDLE = w_handle;
	WM_WFIELD = w_field;
	WM_IX = w2;
	WM_IY = w3;
	WM_IW = w4;
	WM_IH = w5;
	return crys_if(WIND_SET);
}
