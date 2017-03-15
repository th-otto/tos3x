#include "vdilib.h"

int16_t vq_tabstatus(P(int16_t) handle)
PP(int16_t handle;)
{
	contrl[0] = 5;
	contrl[1] = 0;
	contrl[3] = 0;
	contrl[5] = 16;
	contrl[6] = handle;
	vdi();
	return intout[0];
}
