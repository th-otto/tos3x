#include "vdilib.h"

int16_t vsm_height(P(int16_t) handle, P(int16_t) height)
PP(int16_t handle;)
PP(int16_t height;)
{
	ptsin[0] = 0;
	ptsin[1] = height;

	contrl[0] = 19;
	contrl[1] = 1;
	contrl[3] = 0;
	contrl[6] = handle;
	vdi();
	return ptsout[1];
}
