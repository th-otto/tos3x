#include "aeslib.h"

int16_t shel_read(P(char *) pcmd, P(char *) ptail)
PP(char *pcmd;)
PP(char *ptail;)
{
	SH_PCMD = pcmd;
	SH_PTAIL = ptail;
	return crys_if(SHEL_READ);
}
