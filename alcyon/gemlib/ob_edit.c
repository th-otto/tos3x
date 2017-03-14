#include "aeslib.h"

int16_t objc_edit(P(OBJECT *) tree, P(int16_t) obj, P(int16_t) inchar, P(int16_t *) idx, P(int16_t) kind)
PP(OBJECT *tree;)
PP(int16_t obj;)
PP(int16_t inchar;)
PP(int16_t *idx;)
PP(int16_t kind;)
{
	OB_TREE = tree;
	OB_OBJ = obj;
	OB_CHAR = inchar;
	OB_IDX = *idx;
	OB_KIND = kind;
	crys_if(OBJC_EDIT);
	*idx = OB_ODX;
	return RET_CODE;
}
