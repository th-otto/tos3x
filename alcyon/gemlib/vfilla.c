#include "vdilib.h"

VOID v_fillarea(P(int16_t) handle, P(int16_t) count, P(int16_t *) xy)
PP(int16_t handle;)
PP(int16_t count;)
PP(int16_t *xy;)
{
	i_ptsin(xy);

	contrl[0] = 9;
	contrl[1] = count;
	contrl[3] = 0;
	contrl[6] = handle;
	vdi();

	i_ptsin(ptsin);
}
