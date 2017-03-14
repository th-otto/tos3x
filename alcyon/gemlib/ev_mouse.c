#include "aeslib.h"

int16_t evnt_mouse(
	P(int16_t) flags,
	P(int16_t) x, P(int16_t) y,
	P(int16_t) width, P(int16_t) height,
	P(int16_t *) pmx, P(int16_t *) pmy,
	P(int16_t *) pmb, P(int16_t *) pks)
PP(int16_t flags;)
PP(int16_t x;)
PP(int16_t y;)
PP(int16_t width;)
PP(int16_t height;)
PP(int16_t *pmx;)
PP(int16_t *pmy;)
PP(int16_t *pmb;)
PP(int16_t *pks;)
{
	MO_FLAGS = flags;
	MO_X = x;
	MO_Y = y;
	MO_WIDTH = width;
	MO_HEIGHT = height;
	crys_if(EVNT_MOUSE);
	*pmx = EV_MX;
	*pmy = EV_MY;
	*pmb = EV_MB;
	*pks = EV_KS;
	return RET_CODE;
}
