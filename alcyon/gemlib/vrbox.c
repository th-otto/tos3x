#include "vdilib.h"

VOID v_rbox(P(int16_t) handle, P(int16_t *) xy)
PP(int16_t handle;)
PP(int16_t *xy;)
{
	i_ptsin(xy);

	contrl[0] = 11;
	contrl[1] = 2;
	contrl[3] = 0;
	contrl[5] = 8;
	contrl[6] = handle;
	vdi();

	i_ptsin(ptsin);
}
