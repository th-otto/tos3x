#include "aeslib.h"

int16_t appl_tplay(P(VOIDPTR) tbuffer, P(int16_t) tlength, P(int16_t) tscale)
PP(int32_t tbuffer;)
PP(int16_t tlength;)
PP(int16_t tscale;)
{
	AP_TBUFFER = tbuffer;
	AP_TLENGTH = tlength;
	AP_TSCALE = tscale;
	return crys_if(APPL_TPLAY);
}
