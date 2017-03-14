#include "aeslib.h"

int16_t graf_mkstate(P(int16_t *) pmx, P(int16_t *) pmy, P(int16_t *) pmstate, P(int16_t *) pkstate)
PP(int16_t *pmx;)
PP(int16_t *pmy;)
PP(int16_t *pmstate;)
PP(int16_t *pkstate;)
{
	crys_if(GRAF_MKSTATE);
	*pmx = GR_MX;
	*pmy = GR_MY;
	*pmstate = GR_MSTATE;
	*pkstate = GR_KSTATE;
	return RET_CODE;
}
