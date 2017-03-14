#include "aeslib.h"

int16_t form_button(P(OBJECT *) tree, P(int16_t) obj, P(int16_t) clicks, P(int16_t *) nxtobj)
PP(OBJECT *tree;)
PP(int16_t obj;)
PP(int16_t clicks;)
PP(int16_t *nxtobj;)
{
	FM_FORM = tree;
	FM_OBJ = obj;
	FM_CLKS = clicks;
	crys_if(FORM_BUTTON);
	*nxtobj = FM_ONXTOB;
	return RET_CODE;
}
