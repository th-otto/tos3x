#include "aeslib.h"

int16_t shel_write(P(int16_t) doex, P(int16_t) isgr, P(int16_t) iscr, P(const VOIDPTR) pcmd, P(const char *) ptail)
PP(int16_t doex;)
PP(int16_t isgr;)
PP(int16_t iscr;)
PP(const VOIDPTR pcmd;)
PP(const char *ptail;)
{
	SH_DOEX = doex;
	SH_ISGR = isgr;
	SH_ISCR = iscr;
	SH_PCMD = NO_CONST(pcmd);
	SH_PTAIL = NO_CONST(ptail);
	return crys_if(SHEL_WRITE);
}
