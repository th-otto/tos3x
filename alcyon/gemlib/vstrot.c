#include "vdilib.h"

int16_t vst_rotation(P(int16_t) handle, P(int16_t) angle)
PP(int16_t handle;)
PP(int16_t angle;)
{
	intin[0] = angle;

	contrl[0] = 13;
	contrl[1] = 0;
	contrl[3] = 1;
	contrl[6] = handle;
	vdi();
	return intout[0];
}
