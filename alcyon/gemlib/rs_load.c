#include "aeslib.h"

int16_t rsrc_load(P(const char *) rsname)
PP(const char *rsname;)
{
	RS_PFNAME = NO_CONST(rsname);
	return crys_if(RSRC_LOAD);
}
