#include "aeslib.h"

int16_t menu_istart(P(int16_t) flag, P(OBJECT *) tree, P(int16_t) menu, P(int16_t) item)
PP(int16_t flag;)
PP(OBJECT *tree;)
PP(int16_t menu;)
PP(int16_t item;)
{
	M_FLAG = flag;
	M_TREE = tree;
	M_MENU2 = menu;	
	M_ITEM2 = item;

	return crys_if(MENU_ISTART);
}
