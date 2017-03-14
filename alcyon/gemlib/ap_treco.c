#include "aeslib.h"

int16_t appl_trecord(P(VOIDPTR) tbuffer, P(int16_t) tlength)
PP(VOIDPTR tbuffer;)
PP(int16_t tlength;)
{
	AP_TBUFFER = tbuffer;
	AP_TLENGTH = tlength;
	return crys_if(APPL_TRECORD);
}
