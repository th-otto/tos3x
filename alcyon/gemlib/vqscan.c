#include "vdilib.h"

VOID vq_scan(P(int16_t) handle, P(int16_t *) g_height, P(int16_t *) g_slice, P(int16_t *) a_height, P(int16_t *) a_slice, P(int16_t *) factor)
PP(int16_t handle;)
PP(int16_t *g_height;)
PP(int16_t *g_slice;)
PP(int16_t *a_height;)
PP(int16_t *a_slice;)
PP(int16_t *factor;)
{
	contrl[0] = 5;
	contrl[1] = contrl[3] = 0;
	contrl[5] = 24;
	contrl[6] = handle;
	vdi();

	*g_height = intout[0];
	*g_slice = intout[1];
	*a_height = intout[2];
	*a_slice = intout[3];
	*factor = intout[4];
}
