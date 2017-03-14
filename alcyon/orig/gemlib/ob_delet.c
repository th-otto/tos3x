#include "aeslib.h"

int16_t objc_delete(P(OBJECT *) tree, P(int16_t) delob)
PP(OBJECT *tree;)
PP(int16_t delob;)
{
	OB_TREE = tree;
	OB_DELOB = delob;
	return crys_if(OBJC_DELETE);
}
