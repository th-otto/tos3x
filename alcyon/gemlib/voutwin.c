#include "vdilib.h"

VOID v_output_window(P(int16_t) handle, P(int16_t *)xy)
PP(int16_t handle;)
PP(int16_t *xy;)
{
	i_ptsin(xy);

	contrl[0] = 5;
	contrl[1] = 2;
	contrl[3] = 0;
	contrl[5] = 21;
	contrl[6] = handle;
	vdi();

	i_ptsin(ptsin);
}
