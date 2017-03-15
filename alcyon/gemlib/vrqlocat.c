#include "vdilib.h"

VOID vrq_locator(P(int16_t) handle, P(int16_t) initx, P(int16_t) inity, P(int16_t *) xout, P(int16_t *) yout, P(int16_t *) term)
PP(int16_t handle;)
PP(int16_t initx;)
PP(int16_t inity;)
PP(int16_t *xout;)
PP(int16_t *yout;)
PP(int16_t *term;)
{
	ptsin[0] = initx;
	ptsin[1] = inity;

	contrl[0] = 28;
	contrl[1] = 1;
	contrl[3] = 0;
	contrl[6] = handle;
	vdi();

	*xout = ptsout[0];
	*yout = ptsout[1];
	*term = intout[0];
}
