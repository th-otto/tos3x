#include "aeslib.h"

int16_t wind_close(P(int16_t) handle)
PP(int16_t handle;)
{
	WM_HANDLE = handle;
	return crys_if(WIND_CLOSE);
}
