#include "aeslib.h"

int16_t graf_mbox(P(int16_t) w, P(int16_t) h, P(int16_t) srcx, P(int16_t) srcy, P(int16_t) dstx, P(int16_t) dsty)
PP(int16_t w;)
PP(int16_t h;)
PP(int16_t srcx;)
PP(int16_t srcy;)
PP(int16_t dstx;)
PP(int16_t dsty;)
{
	GR_I1 = w;
	GR_I2 = h;
	GR_I3 = srcx;
	GR_I4 = srcy;
	GR_I5 = dstx;
	GR_I6 = dsty;
	return crys_if(GRAF_MBOX);
}
