#include "aeslib.h"

int16_t menu_ienable(P(OBJECT *) tree, P(int16_t) itemnum, P(int16_t) enableit)
PP(OBJECT *tree;)
PP(int16_t itemnum;)
PP(int16_t enableit;)
{
	MM_ITREE = tree;
	ITEM_NUM = itemnum;
	ENABLE_IT = enableit;
	return crys_if(MENU_IENABLE);
}
