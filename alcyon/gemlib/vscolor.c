#include "vdilib.h"

VOID vs_color(P(int16_t) handle, P(int16_t) index, P(int16_t *) rgb)
PP(int16_t handle;)
PP(int16_t index;)
PP(int16_t *rgb;)
{
	int16_t i;

	intin[0] = index;
	for (i = 1; i < 4; i++)
		intin[i] = *rgb++;

	contrl[0] = 14;
	contrl[1] = 0;
	contrl[3] = 4;
	contrl[6] = handle;
	vdi();
}
