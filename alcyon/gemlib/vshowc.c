#include "vdilib.h"

VOID v_show_c(P(int16_t) handle, P(int16_t) reset)
PP(int16_t handle;)
PP(int16_t reset;)
{
	intin[0] = reset;

	contrl[0] = 122;
	contrl[1] = 0;
	contrl[3] = 1;
	contrl[6] = handle;
	vdi();
}
