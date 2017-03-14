#include "aeslib.h"

int16_t objc_offset(P(OBJECT *) tree, P(int16_t) obj, P(int16_t *) poffx, P(int16_t *) poffy)
PP(OBJECT *tree;)
PP(int16_t obj;)
PP(int16_t *poffx;)
PP(int16_t *poffy;)
{
	OB_TREE = tree;
	OB_OBJ = obj;
	crys_if(OBJC_OFFSET);
	*poffx = OB_XOFF;
	*poffy = OB_YOFF;
	return RET_CODE;
}
