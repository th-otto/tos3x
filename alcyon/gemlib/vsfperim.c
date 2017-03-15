#include "vdilib.h"

int16_t vsf_perimeter(P(int16_t) handle, P(int16_t) per_vis)
PP(int16_t handle;)
PP(int16_t per_vis;)
{
	intin[0] = per_vis;

	contrl[0] = 104;
	contrl[1] = 0;
	contrl[3] = 1;
	contrl[6] = handle;
	vdi();
	return intout[0];
}
