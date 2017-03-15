#include "vdilib.h"

VOID v_circle(P(int16_t) handle, P(int16_t) xc, P(int16_t) yc, P(int16_t) rad)
PP(int16_t handle;)
PP(int16_t xc;)
PP(int16_t yc;)
PP(int16_t rad;)
{
	ptsin[0] = xc;
	ptsin[1] = yc;
	ptsin[2] = 0;
	ptsin[3] = 0;
	ptsin[4] = rad;
	ptsin[5] = 0;

	contrl[0] = 11;
	contrl[1] = 3;
	contrl[3] = 0;
	contrl[5] = 4;
	contrl[6] = handle;
	vdi();
}

