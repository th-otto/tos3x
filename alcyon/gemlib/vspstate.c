#include "vdilib.h"

VOID vsp_state(P(int16_t) handle, P(int16_t) port, P(int16_t) film, P(int16_t) lightness, P(int16_t) interlace, P(int16_t) planes, P(int16_t *)indexes)
PP(int16_t handle;)
PP(int16_t port;)
PP(int16_t film;)
PP(int16_t lightness;)
PP(int16_t interlace;)
PP(int16_t planes;)
PP(int16_t *indexes;)
{
	int16_t i;

	contrl[0] = 5;
	contrl[1] = 0;
	contrl[3] = 21;
	contrl[5] = 93;
	contrl[6] = handle;

	intin[0] = port;
	intin[1] = film;
	intin[2] = lightness;
	intin[3] = interlace;
	intin[4] = planes;
	for (i = 0; i < 16; i++)
		intin[i + 5] = indexes[i];
	vdi();
}
