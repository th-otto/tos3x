#include "aeslib.h"

int16_t evnt_multi(P(int16_t) flags, P(int16_t) bclk, P(int16_t) bmsk, P(int16_t) bst,
	P(int16_t) m1flags, P(int16_t) m1x, P(int16_t) m1y, P(int16_t) m1w, P(int16_t) m1h,
	P(int16_t) m2flags, P(int16_t) m2x, P(int16_t) m2y, P(int16_t) m2w, P(int16_t) m2h,
	P(int16_t *) mepbuff,
	P(int16_t) tlc, P(int16_t) thc,
	P(int16_t *) pmx, P(int16_t *) pmy, P(int16_t *) pmb, P(int16_t *) pks, P(int16_t *) pkr, P(int16_t *) pbr)
PP(int16_t flags;)
PP(int16_t bclk;)
PP(int16_t bmsk;)
PP(int16_t bst;)

PP(int16_t m1flags;)
PP(int16_t m1x;)
PP(int16_t m1y;)
PP(int16_t m1w;)
PP(int16_t m1h;)

PP(int16_t m2flags;)
PP(int16_t m2x;)
PP(int16_t m2y;)
PP(int16_t m2w;)
PP(int16_t m2h;)

PP(int16_t *mepbuff;)
PP(int16_t tlc;)
PP(int16_t thc;)

PP(int16_t *pmx;)
PP(int16_t *pmy;)
PP(int16_t *pmb;)
PP(int16_t *pks;)
PP(int16_t *pkr;)
PP(int16_t *pbr;)
{
	MU_FLAGS = flags;

	MB_CLICKS = bclk;
	MB_MASK = bmsk;
	MB_STATE = bst;

	MMO1_FLAGS = m1flags;
	MMO1_X = m1x;
	MMO1_Y = m1y;
	MMO1_WIDTH = m1w;
	MMO1_HEIGHT = m1h;

	MMO2_FLAGS = m2flags;
	MMO2_X = m2x;
	MMO2_Y = m2y;
	MMO2_WIDTH = m2w;
	MMO2_HEIGHT = m2h;

	MME_PBUFF = mepbuff;

	MT_LOCOUNT = tlc;
	MT_HICOUNT = thc;

	crys_if(EVNT_MULTI);

	*pmx = EV_MX;
	*pmy = EV_MY;
	*pmb = EV_MB;
	*pks = EV_KS;
	*pkr = EV_KRET;
	*pbr = EV_BRET;
	return RET_CODE;
}
