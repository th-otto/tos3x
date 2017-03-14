#include "aeslib.h"

int16_t scrp_read(P(char *) pscrap)
PP(char *pscrap;)
{
	SC_PATH = pscrap;
	return crys_if(SCRP_READ);
}
