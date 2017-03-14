#include "aeslib.h"

int16_t shel_envrn(P(char **) ppath, P(const char *) psrch)
PP(char **ppath;)
PP(const char *psrch;)
{
	SH_PATH = ppath;
	SH_SRCH = NO_CONST(psrch);
	return crys_if(SHEL_ENVRN);
}
