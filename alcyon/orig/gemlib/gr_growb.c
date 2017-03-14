#include "aeslib.h"

int16_t graf_growbox(P(int16_t) orgx, P(int16_t) orgy, P(int16_t) orgw, P(int16_t) orgh, P(int16_t) x, P(int16_t) y, P(int16_t) w, P(int16_t) h)
PP(int16_t orgx;)
PP(int16_t orgy;)
PP(int16_t orgw;)
PP(int16_t orgh;)

PP(int16_t x;)
PP(int16_t y;)
PP(int16_t w;)
PP(int16_t h;)
{
	GR_I1 = orgx;
	GR_I2 = orgy;
	GR_I3 = orgw;
	GR_I4 = orgh;
	GR_I5 = x;
	GR_I6 = y;
	GR_I7 = w;
	GR_I8 = h;
	return crys_if(GRAF_GROWBOX);
}
