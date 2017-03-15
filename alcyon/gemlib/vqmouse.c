#include "vdilib.h"

VOID vq_mouse(P(int16_t) handle, P(int16_t *) status, P(int16_t *) px, P(int16_t *) py)
PP(int16_t handle;)
PP(int16_t *status;)
PP(int16_t *px;)
PP(int16_t *py;)
{
	contrl[0] = 124;
	contrl[1] = 0;
	contrl[3] = 0;
	contrl[6] = handle;
	vdi();

	*status = intout[0];
	*px = ptsout[0];
	*py = ptsout[1];
}
