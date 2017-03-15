#include "vdilib.h"

int16_t vsl_width(P(int16_t) handle, P(int16_t) width)
PP(int16_t handle;)
PP(int16_t width;)
{
	ptsin[0] = width;
	ptsin[1] = 0;

	contrl[0] = 16;
	contrl[1] = 1;
	contrl[3] = 0;
	contrl[6] = handle;
	vdi();
	return ptsout[0];
}
