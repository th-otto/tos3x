#include "vdilib.h"

VOID vst_alignment(P(int16_t) handle, P(int16_t) hor_in, P(int16_t) vert_in, P(int16_t *) hor_out, P(int16_t *) vert_out)
PP(int16_t handle;)
PP(int16_t hor_in;)
PP(int16_t vert_in;)
PP(int16_t *hor_out;)
PP(int16_t *vert_out;)
{
	intin[0] = hor_in;
	intin[1] = vert_in;

	contrl[0] = 39;
	contrl[1] = 0;
	contrl[3] = 2;
	contrl[6] = handle;
	vdi();

	*hor_out = intout[0];
	*vert_out = intout[1];
}
