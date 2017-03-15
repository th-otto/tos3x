#include "vdilib.h"

short vq_color(P(int16_t) handle, P(int16_t) index, P(int16_t) set_flag, P(int16_t *) rgb)
PP(int16_t handle;)
PP(int16_t index;)
PP(int16_t set_flag;)
PP(int16_t *rgb;)
{
	intin[0] = index;
	intin[1] = set_flag;

	contrl[0] = 26;
	contrl[1] = 0;
	contrl[3] = 2;
	contrl[6] = handle;
	vdi();

	rgb[0] = intout[1];
	rgb[1] = intout[2];
	rgb[2] = intout[3];
	return intout[0];
}
