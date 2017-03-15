#include "vdilib.h"

int16_t vqp_error(P(int16_t) handle)
PP(int16_t handle;)
{
	contrl[0] = 5;
	contrl[1] = 0;
	contrl[3] = 0;
	contrl[5] = 96;
	contrl[6] = handle;
	vdi();

	return intout[0];
}
