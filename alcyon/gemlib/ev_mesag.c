#include "aeslib.h"

int16_t evnt_mesag(P(int16_t *) pbuff)
PP(int16_t *pbuff;)
{
	ME_PBUFF = pbuff;
	return crys_if(EVNT_MESAG);
}
