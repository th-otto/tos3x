#include "aeslib.h"

int16_t shel_get(P(char *) pbuffer, P(int16_t) len)
PP(char *pbuffer;)
PP(int16_t len;)
{
	SH_PBUFFER = pbuffer;
	SH_LEN = len;
	return crys_if(SHEL_GET);
}
