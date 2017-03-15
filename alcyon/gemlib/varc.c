#include "vdilib.h"

VOID v_arc(P(int16_t) handle, P(int16_t) xc, P(int16_t) yc, P(int16_t) rad, P(int16_t) sang, P(int16_t) eang)
PP(int16_t handle;)
PP(int16_t xc;)
PP(int16_t yc;)
PP(int16_t rad;)
PP(int16_t sang;)
PP(int16_t eang;)
{
	ptsin[0] = xc;
	ptsin[1] = yc;
	ptsin[2] = 0;
	ptsin[3] = 0;
	ptsin[4] = 0;
	ptsin[5] = 0;
	ptsin[6] = rad;
	ptsin[7] = 0;
	intin[0] = sang;
	intin[1] = eang;

	contrl[0] = 11;
	contrl[1] = 4;
	contrl[3] = 2;
	contrl[5] = 2;
	contrl[6] = handle;
	vdi();
}
