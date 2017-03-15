#include "vdilib.h"

int16_t vswr_mode(P(int16_t) handle, P(int16_t) mode)
PP(int16_t handle;)
PP(int16_t mode;)
{
	intin[0] = mode;

	contrl[0] = 32;
	contrl[1] = 0;
	contrl[3] = 1;
	contrl[6] = handle;
	vdi();
	return intout[0];
}
