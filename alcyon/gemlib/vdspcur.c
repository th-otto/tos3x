#include "vdilib.h"

VOID v_dspcur(P(int16_t) handle, P(int16_t) x, P(int16_t) y)
PP(int16_t handle;)
PP(int16_t x;)
PP(int16_t y;)
{
	ptsin[0] = x;
	ptsin[1] = y;

	contrl[0] = 5;
	contrl[1] = 1;
	contrl[3] = 0;
	contrl[5] = 18;
	contrl[6] = handle;
	vdi();
}
