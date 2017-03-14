#include "aeslib.h"

int16_t evnt_button(P(int16_t) clicks, P(int16_t) mask, P(int16_t) state, P(int16_t *)pmx, P(int16_t *)pmy, P(int16_t *)pmb, P(int16_t *)pks)
PP(int16_t clicks;)
PP(int16_t mask;)
PP(int16_t state;)
PP(int16_t *pmx;)
PP(int16_t *pmy;)
PP(int16_t *pmb;)
PP(int16_t *pks;)
{
	B_CLICKS = clicks;
	B_MASK = mask;
	B_STATE = state;
	crys_if(EVNT_BUTTON);
	*pmx = EV_MX;
	*pmy = EV_MY;
	*pmb = EV_MB;
	*pks = EV_KS;
	return RET_CODE;
}
