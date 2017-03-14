#include "aeslib.h"

int16_t form_center(P(OBJECT *) tree, P(int16_t *) pcx, P(int16_t *) pcy, P(int16_t *) pcw, P(int16_t *) pch)
PP(OBJECT *tree;)
PP(int16_t *pcx;)
PP(int16_t *pcy;)
PP(int16_t *pcw;)
PP(int16_t *pch;)
{
	FM_FORM = tree;
	crys_if(FORM_CENTER);
	*pcx = FM_XC;
	*pcy = FM_YC;
	*pcw = FM_WC;
	*pch = FM_HC;
	return RET_CODE;
}
