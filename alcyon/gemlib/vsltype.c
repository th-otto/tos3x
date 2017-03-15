#include "vdilib.h"

int16_t vsl_type(P(int16_t) handle, P(int16_t) style)
PP(int16_t handle;)
PP(int16_t style;)
{
	intin[0] = style;

	contrl[0] = 15;
	contrl[1] = 0;
	contrl[3] = 1;
	contrl[6] = handle;
	vdi();
	return intout[0];
}
