#include "vdilib.h"

VOID v_updwk(P(int16_t) handle)
PP(int16_t handle;)
{
	contrl[0] = 4;
	contrl[1] = 0;
	contrl[3] = 0;
	contrl[6] = handle;
	vdi();
}
