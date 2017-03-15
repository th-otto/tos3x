#include "vdilib.h"

VOID v_rvon(P(int16_t) handle)
PP(int16_t handle;)
{
	contrl[0] = 5;
	contrl[1] = 0;
	contrl[3] = 0;
	contrl[5] = 13;
	contrl[6] = handle;
	vdi();
}
