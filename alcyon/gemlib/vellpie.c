#include "vdilib.h"

VOID v_ellpie(P(int16_t) handle, P(int16_t) xc, P(int16_t) yc, P(int16_t) xrad, P(int16_t) yrad, P(int16_t) sang, P(int16_t) eang)
PP(int16_t handle;)
PP(int16_t xc;)
PP(int16_t yc;)
PP(int16_t xrad;)
PP(int16_t yrad;)
PP(int16_t sang;)
PP(int16_t eang;)
{
	ptsin[0] = xc;
	ptsin[1] = yc;
	ptsin[2] = xrad;
	ptsin[3] = yrad;
	intin[0] = sang;
	intin[1] = eang;

	contrl[0] = 11;
	contrl[1] = 2;
	contrl[3] = 2;
	contrl[5] = 7;
	contrl[6] = handle;
	vdi();
}
