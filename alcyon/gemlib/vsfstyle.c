#include "vdilib.h"

int16_t vsf_style(P(int16_t) handle, P(int16_t) index)
PP(int16_t handle;)
PP(int16_t index;)
{
	intin[0] = index;

	contrl[0] = 24;
	contrl[1] = 0;
	contrl[3] = 1;
	contrl[6] = handle;
	vdi();
	return intout[0];
}
