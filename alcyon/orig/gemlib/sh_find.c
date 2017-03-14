#include "aeslib.h"

int16_t shel_find(P(char *) ppath)
PP(char *ppath;)
{
	SH_PATH = ppath;
	return crys_if(SHEL_FIND);
}
