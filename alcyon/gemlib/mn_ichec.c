#include "aeslib.h"

int16_t menu_icheck(P(OBJECT *) tree, P(int16_t) itemnum, P(int16_t) checkit)
PP(OBJECT *tree;)
PP(int16_t itemnum;)
PP(int16_t checkit;)
{
	MM_ITREE = tree;
	ITEM_NUM = itemnum;
	CHECK_IT = checkit;
	return crys_if(MENU_ICHECK);
}
