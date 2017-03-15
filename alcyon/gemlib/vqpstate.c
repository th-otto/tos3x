#include "vdilib.h"

VOID vqp_state(P(int16_t) handle, P(int16_t *)port, P(int16_t *)film, P(int16_t *)lightness, P(int16_t *)interlace, P(int16_t *)planes, P(int16_t *)indexes)
PP(int16_t handle;)
PP(int16_t *port;)
PP(int16_t *film;)
PP(int16_t *lightness;)
PP(int16_t *interlace;)
PP(int16_t *planes;)
PP(int16_t *indexes;)
{
	int16_t i;

	contrl[0] = 5;
	contrl[1] = 0;
	contrl[3] = 0;
	contrl[5] = 92;
	contrl[6] = handle;
	vdi();

	*port = intout[0];
	*film = intout[1];
	*lightness = intout[2];
	*interlace = intout[3];
	*planes = intout[4];
	for (i = 0; i < 16; i++)
		indexes[i] = intout[i + 5];
}
