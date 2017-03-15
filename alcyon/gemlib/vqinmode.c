#include "vdilib.h"

VOID vqin_mode(P(int16_t) handle, P(int16_t) dev_type, P(int16_t *) mode)
PP(int16_t handle;)
PP(int16_t dev_type;)
PP(int16_t *mode;)
{
	intin[0] = dev_type;

	contrl[0] = 115;
	contrl[1] = 0;
	contrl[3] = 1;
	contrl[6] = handle;
	vdi();

	*mode = intout[0];
}
