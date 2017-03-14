#include "aeslib.h"

int16_t objc_find(P(OBJECT *) tree, P(int16_t) startob, P(int16_t) depth, P(int16_t) mx, P(int16_t) my)
PP(OBJECT *tree;)
PP(int16_t startob;)
PP(int16_t depth;)
PP(int16_t mx;)
PP(int16_t my;)
{
	OB_TREE = tree;
	OB_STARTOB = startob;
	OB_DEPTH = depth;
	OB_MX = mx;
	OB_MY = my;
	return crys_if(OBJC_FIND);
}
