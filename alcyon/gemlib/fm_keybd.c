#include "aeslib.h"

int16_t form_keybd(P(OBJECT *) tree, P(int16_t) obj, P(int16_t) next, P(int16_t) kchar, P(int16_t *) nxtobj, P(int16_t *) nxtchar)
PP(OBJECT *tree;)
PP(int16_t obj;)
PP(int16_t next;)
PP(int16_t kchar;)
PP(int16_t *nxtobj;)
PP(int16_t *nxtchar;)
{
	FM_FORM = tree;
	FM_OBJ = obj;
	FM_ICHAR = kchar;
	FM_INXTOB = next;
	crys_if(FORM_KEYBD);
	*nxtobj = FM_ONXTOB;
	*nxtchar = FM_OCHAR;
	return RET_CODE;
}
