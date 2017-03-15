#include "vdilib.h"

VOID v_write_meta(P(int16_t) handle, P(int16_t) num_ints, P(int16_t *) ints, P(int16_t) num_pts, P(int16_t *) pts)
PP(int16_t handle;)
PP(int16_t num_ints;)
PP(int16_t *ints;)
PP(int16_t num_pts;)
PP(int16_t *pts;)
{
	i_intin(ints);
	i_ptsin(pts);

	contrl[0] = 5;
	contrl[1] = num_pts;
	contrl[3] = num_ints;
	contrl[5] = 99;
	contrl[6] = handle;
	vdi();

	i_intin(intin);
	i_ptsin(ptsin);
}
