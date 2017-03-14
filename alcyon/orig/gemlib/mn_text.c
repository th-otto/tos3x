#include "aeslib.h"

int16_t menu_text(P(OBJECT *) tree, P(int16_t) inum, P(const char *) ptext)
PP(OBJECT *tree;)
PP(int16_t inum;)
PP(const char *ptext;)
{
	MM_ITREE = tree;
	ITEM_NUM = inum;
	MM_PTEXT = NO_CONST(ptext);
	return crys_if(MENU_TEXT);
}
