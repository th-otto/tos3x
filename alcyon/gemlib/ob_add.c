#include "aeslib.h"

int16_t objc_add(P(OBJECT *) tree, P(int16_t) parent, P(int16_t) child)
PP(OBJECT *tree;)
PP(int16_t parent;)
PP(int16_t child;)
{
	OB_TREE = tree;
	OB_PARENT = parent;
	OB_CHILD = child;
	return crys_if(OBJC_ADD);
}
