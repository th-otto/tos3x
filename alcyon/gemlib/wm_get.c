#include "aeslib.h"

int16_t wind_get(P(int16_t) w_handle, P(int16_t) w_field, P(int16_t *) pw1, P(int16_t *) pw2, P(int16_t *) pw3, P(int16_t *) pw4)
PP(int16_t w_handle;)
PP(int16_t w_field;)
PP(int16_t *pw1;)
PP(int16_t *pw2;)
PP(int16_t *pw3;)
PP(int16_t *pw4;)
{
	WM_HANDLE = w_handle;
	WM_WFIELD = w_field;
	crys_if(WIND_GET);
	*pw1 = WM_OX;
	*pw2 = WM_OY;
	*pw3 = WM_OW;
	*pw4 = WM_OH;
	return RET_CODE;
}
