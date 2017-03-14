#include "aeslib.h"

int16_t rsrc_saddr(P(int16_t) rstype, P(int16_t) rsid, P(VOIDPTR) lngval)
PP(int16_t rstype;)
PP(int16_t rsid;)
PP(VOIDPTR lngval;)
{
	RS_TYPE = rstype;
	RS_INDEX = rsid;
	RS_INADDR = lngval;
	return crys_if(RSRC_SADDR);
}
