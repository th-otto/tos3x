#include "vdilib.h"

VOID v_ellipse(P(int16_t) handle, P(int16_t) xc, P(int16_t) yc, P(int16_t) xrad, P(int16_t) yrad)
PP(int16_t handle;)
PP(int16_t xc;)
PP(int16_t yc;)
PP(int16_t xrad;)
PP(int16_t yrad;)
{
	ptsin[0] = xc;
	ptsin[1] = yc;
	ptsin[2] = xrad;
	ptsin[3] = yrad;

	contrl[0] = 11;
	contrl[1] = 2;
	contrl[3] = 0;
	contrl[5] = 5;
	contrl[6] = handle;
	vdi();
}

