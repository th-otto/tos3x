#include "aeslib.h"

int16_t menu_attach(P(int16_t) flag, P(OBJECT *) tree, P(int16_t) item, P(MENU *) menu)
PP(int16_t flag;)
PP(OBJECT *tree;)
PP(int16_t item;)
PP(MENU *menu;)
{
	M_FLAG = flag;
	M_TREE = tree;
	M_ITEM = item;
	M_MDATA = menu;

	return crys_if(MENU_ATTACH);
}
