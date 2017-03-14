#include "aeslib.h"

int16_t menu_tnormal(P(OBJECT *) tree, P(int16_t) titlenum, P(int16_t) normalit)
PP(OBJECT *tree;)
PP(int16_t titlenum;)
PP(int16_t normalit;)
{
	MM_ITREE = tree;
	TITLE_NUM = titlenum;
	NORMAL_IT = normalit;
	return crys_if(MENU_TNORMAL);
}
