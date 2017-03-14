#include "aeslib.h"

int16_t appl_write(P(int16_t) rwid, P(int16_t) length, P(const VOIDPTR) pbuff)
PP(int16_t rwid;)
PP(int16_t length;)
PP(const VOIDPTR pbuff;)
{
	AP_RWID = rwid;
	AP_LENGTH = length;
	AP_PBUFF = NO_CONST(pbuff);
	return crys_if(APPL_WRITE);
}
