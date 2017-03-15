#include "vdilib.h"

VOID v_clsvwk(P(int16_t) handle)
PP(int16_t handle;)
{
	contrl[0] = 101;
	contrl[1] = 0;
	contrl[3] = 0;
	contrl[6] = handle;
	vdi();
}

