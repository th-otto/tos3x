#include "vdilib.h"

VOID v_contourfill(P(int16_t) handle, P(int16_t) x, P(int16_t) y, P(int16_t) idx)
PP(int16_t handle;)
PP(int16_t x;)
PP(int16_t y;)
PP(int16_t idx;)
{
	intin[0] = idx;
	ptsin[0] = x;
	ptsin[1] = y;

	contrl[0] = 103;
	contrl[1] = 1;
	contrl[3] = 1;
	contrl[6] = handle;
	vdi();
}

