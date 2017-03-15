#include "vdilib.h"

VOID vrq_valuator(P(int16_t) handle, P(int16_t) val_in, P(int16_t *) val_out, P(int16_t *) term)
PP(int16_t handle;)
PP(int16_t val_in;)
PP(int16_t *val_out;)
PP(int16_t *term;)
{
	intin[0] = val_in;

	contrl[0] = 29;
	contrl[1] = 0;
	contrl[3] = 1;
	contrl[6] = handle;
	vdi();

	*val_out = intout[0];
	*term = intout[1];
}
