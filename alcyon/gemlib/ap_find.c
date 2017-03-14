#include "aeslib.h"

int16_t appl_find(P(const char *) pname)
PP(const char *pname;)
{
	AP_PNAME = NO_CONST(pname);
	return crys_if(APPL_FIND);
}
