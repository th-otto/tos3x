#include "vdilib.h"

int16_t vs_palette(P(int16_t) handle, P(int16_t) palette)
PP(int16_t handle;)
PP(int16_t palette;)
{
	contrl[0] = 5;
	contrl[1] = 0;
	contrl[3] = 1;
	contrl[5] = 60;
	contrl[6] = handle;
	intin[0] = palette;
	vdi();
	return intout[0];
}
