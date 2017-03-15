#include "vdilib.h"

short vsin_mode(P(int16_t) handle, P(int16_t) dev_type, P(int16_t) mode)
PP(int16_t handle;)
PP(int16_t dev_type;)
PP(int16_t mode;)
{
	intin[0] = dev_type;
	intin[1] = mode;

	contrl[0] = 33;
	contrl[1] = 0;
	contrl[3] = 2;
	contrl[6] = handle;
	vdi();
	return intout[0];
}
