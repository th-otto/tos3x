#include "vdilib.h"

VOID vq_key_s(P(int16_t) handle, P(int16_t *) status)
PP(int16_t handle;)
PP(int16_t *status;)
{
	contrl[0] = 128;
	contrl[1] = 0;
	contrl[3] = 0;
	contrl[6] = handle;
	vdi();

	*status = intout[0];
}
