#include "aeslib.h"

int16_t graf_watchbox(P(OBJECT *) tree, P(int16_t) obj, P(int16_t) instate, P(int16_t) outstate)
PP(OBJECT *tree;)
PP(int16_t obj;)
PP(int16_t instate;)
PP(int16_t outstate;)
{
	GR_TREE = tree;
	GR_OBJ = obj;
	GR_INSTATE = instate;
	GR_OUTSTATE = outstate;
	return crys_if(GRAF_WATCHBOX);
}
