#include "aeslib.h"

int16_t appl_read(P(int16_t) rwid, P(int16_t) length, P(VOIDPTR) pbuff)
PP(int16_t rwid;)
PP(int16_t length;)
PP(int32_t pbuff;)
{
	AP_RWID = rwid;
	AP_LENGTH = length;
	AP_PBUFF = pbuff;
	return crys_if(APPL_READ);
}
