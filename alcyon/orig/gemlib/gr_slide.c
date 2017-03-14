#include "aeslib.h"

int16_t graf_slidebox(P(OBJECT *) tree, P(int16_t) parent, P(int16_t) obj, P(int16_t) isvert)
PP(OBJECT *tree;)
PP(int16_t parent;)
PP(int16_t obj;)
PP(int16_t isvert;)
{
	GR_TREE = tree;
	GR_PARENT = parent;
	GR_OBJ = obj;
	GR_ISVERT = isvert;
	return crys_if(GRAF_SLIDEBOX);
}
