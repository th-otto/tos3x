#include "aeslib.h"

int16_t scrp_write(P(const char *) pscrap)
PP(const char *pscrap;)
{
	SC_PATH = NO_CONST(pscrap);
	return crys_if(SCRP_WRITE);
}
