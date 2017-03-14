#include "aeslib.h"

int16_t form_dial(P(int16_t) dtype, P(int16_t) ix, P(int16_t) iy, P(int16_t) iw, P(int16_t) ih, P(int16_t) x, P(int16_t) y, P(int16_t) w, P(int16_t) h)
PP(int16_t dtype;)
PP(int16_t ix;)
PP(int16_t iy;)
PP(int16_t iw;)
PP(int16_t ih;)
PP(int16_t x;)
PP(int16_t y;)
PP(int16_t w;)
PP(int16_t h;)
{
	FM_TYPE = dtype;
	FM_IX = ix;
	FM_IY = iy;
	FM_IW = iw;
	FM_IH = ih;
	FM_X = x;
	FM_Y = y;
	FM_W = w;
	FM_H = h;
	return crys_if(FORM_DIAL);
}
