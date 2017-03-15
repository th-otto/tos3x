#include "vdilib.h"

VOID vsp_film(P(int16_t) handle, P(int16_t) index, P(int16_t) exposure)
PP(int16_t handle;)
PP(int16_t index;)
PP(int16_t exposure;)
{
	contrl[0] = 5;
	contrl[1] = 0;
	contrl[3] = 2;
	contrl[5] = 91;
	contrl[6] = handle;
	intin[0] = index;
	intin[1] = exposure;
	vdi();
}
