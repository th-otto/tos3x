#include "aeslib.h"

int16_t objc_sysvar(P(int16_t) mode, P(int16_t) which, P(int16_t) inval1, P(int16_t) inval2, P(int16_t *) outval1, P(int16_t *) outval2)
PP(int16_t mode;)
PP(int16_t which;)
PP(int16_t inval1;)
PP(int16_t inval2;)
PP(int16_t *outval1;)
PP(int16_t *outval2;)
{
	OB_MODE = mode;
	OB_WHICH = which;
	OB_I1 = inval1;
	OB_I2 = inval2;
	crys_if(OBJC_SYSVAR);
	*outval1 = OB_O1;
	*outval2 = OB_O2;
	return RET_CODE;
}
