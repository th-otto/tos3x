#include "vdilib.h"

VOID v_get_pixel(P(int16_t) handle, P(int16_t) x, P(int16_t) y, P(int16_t *) pel, P(int16_t *) index)
PP(int16_t handle;)
PP(int16_t x;)
PP(int16_t y;)
PP(int16_t *pel;)
PP(int16_t *index;)
{
	ptsin[0] = x;
	ptsin[1] = y;

	contrl[0] = 105;
	contrl[1] = 1;
	contrl[3] = 0;
	contrl[6] = handle;
	vdi();

	*pel = intout[0];
	*index = intout[1];
}
