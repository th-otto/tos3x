#include "aeslib.h"

int16_t graf_dragbox(
	P(int16_t) w, P(int16_t) h, P(int16_t) sx, P(int16_t) sy,
	P(int16_t) xc, P(int16_t) yc, P(int16_t) wc, P(int16_t) hc,
	P(int16_t *) pdx, P(int16_t *) pdy)
PP(int16_t w;)
PP(int16_t h;)
PP(int16_t sx;)
PP(int16_t sy;)

PP(int16_t xc;)
PP(int16_t yc;)
PP(int16_t wc;)
PP(int16_t hc;)

PP(int16_t *pdx;)
PP(int16_t *pdy;)
{
	GR_I1 = w;
	GR_I2 = h;
	GR_I3 = sx;
	GR_I4 = sy;
	GR_I5 = xc;
	GR_I6 = yc;
	GR_I7 = wc;
	GR_I8 = hc;
	crys_if(GRAF_DRAGBOX);
	*pdx = GR_O1;
	*pdy = GR_O2;
	return RET_CODE;
}
