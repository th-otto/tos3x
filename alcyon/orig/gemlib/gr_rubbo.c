#include "aeslib.h"

int16_t graf_rubbox(P(int16_t) xorigin, P(int16_t) yorigin, P(int16_t) wmin, P(int16_t) hmin, P(int16_t *) pwend, P(int16_t *) phend)
PP(int16_t xorigin;)
PP(int16_t yorigin;)
PP(int16_t wmin;)
PP(int16_t hmin;)
PP(int16_t *pwend;)
PP(int16_t *phend;)
{
	GR_I1 = xorigin;
	GR_I2 = yorigin;
	GR_I3 = wmin;
	GR_I4 = hmin;
	crys_if(GRAF_RUBBOX);
	*pwend = GR_O1;
	*phend = GR_O2;
	return RET_CODE;
}
