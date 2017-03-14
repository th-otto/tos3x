#include "aeslib.h"

int16_t menu_bar(P(OBJECT *) tree, P(int16_t) showit)
PP(OBJECT *tree;)
PP(int16_t showit;)
{
	MM_ITREE = tree;
	SHOW_IT = showit;
	return crys_if(MENU_BAR);
}
