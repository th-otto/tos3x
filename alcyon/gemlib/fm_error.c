#include "aeslib.h"

int16_t form_error(P(int16_t) errnum)
PP(int16_t errnum;)
{
	FM_ERRNUM = errnum;
	return crys_if(FORM_ERROR);
}
