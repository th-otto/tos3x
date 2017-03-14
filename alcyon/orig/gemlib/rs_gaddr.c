#include "aeslib.h"

int16_t rsrc_gaddr(P(int16_t) rstype, P(int16_t) rsid, P(VOIDPTR) paddr)
PP(int16_t rstype;)
PP(int16_t rsid;)
PP(VOIDPTR paddr;)
{
	RS_TYPE = rstype;
	RS_INDEX = rsid;
	crys_if(RSRC_GADDR);
	*(VOIDPTR **)paddr = RS_OUTADDR;
	return RET_CODE;
}
