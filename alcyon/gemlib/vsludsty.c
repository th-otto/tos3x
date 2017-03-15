#include "vdilib.h"

VOID vsl_udsty(P(int16_t) handle, P(int16_t) pattern)
PP(int16_t handle;)
PP(int16_t pattern;)
{
	intin[0] = pattern;

	contrl[0] = 113;
	contrl[1] = 0;
	contrl[3] = 1;
	contrl[6] = handle;
	vdi();
}
