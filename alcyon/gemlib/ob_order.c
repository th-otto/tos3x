#include "aeslib.h"

int16_t objc_order(P(OBJECT *) tree, P(int16_t) mov_obj, P(int16_t) newpos)
PP(OBJECT *tree;)
PP(int16_t mov_obj;)
PP(int16_t newpos;)
{
	OB_TREE = tree;
	OB_OBJ = mov_obj;
	OB_NEWPOS = newpos;
	return crys_if(OBJC_ORDER);
}
