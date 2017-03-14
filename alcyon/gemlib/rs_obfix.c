#include "aeslib.h"

int16_t rsrc_obfix(P(OBJECT *) tree, P(int16_t) obj)
PP(OBJECT *tree;)
PP(int16_t obj;)
{
	RS_TREE = tree;
	RS_OBJ = obj;
	return crys_if(RSRC_OBFIX);
}
