#include "aeslib.h"

int16_t appl_init(NOTHING)
{
	crys_if(APPL_INIT);
	gl_apid = RET_CODE;
	return gl_apid;
}
