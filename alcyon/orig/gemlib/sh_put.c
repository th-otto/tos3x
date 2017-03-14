#include "aeslib.h"

int16_t shel_put(P(const char *) pdata, P(int16_t) len)
PP(const char *pdata;)
PP(int16_t len;)
{
	SH_PDATA = NO_CONST(pdata);
	SH_LEN = len;
	return crys_if(SHEL_PUT);
}
