#include "vdilib.h"

int16_t vs_mute(P(int16_t) handle, P(int16_t) action)
PP(int16_t handle;)
PP(int16_t action;)
{
	contrl[0] = 5;
	contrl[1] = 0;
	contrl[3] = 1;
	contrl[5] = 62;
	contrl[6] = handle;

	intin[0] = action;
	vdi();
	return intout[0];
}
