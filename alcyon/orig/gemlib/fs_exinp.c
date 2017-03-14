#include "aeslib.h"

int16_t fsel_exinput(P(char *) pipath, P(char *) pisel, P(int16_t *) pbutton, P(const char *) label)
PP(char *pipath;)
PP(char *pisel;)
PP(int16_t *pbutton;)
PP(const char *label;)
{
	FS_IPATH = pipath;
	FS_ISEL = pisel;
	FS_LABEL = NO_CONST(label);
	crys_if(FSEL_EXINPUT);
	*pbutton = FS_BUTTON;
	return RET_CODE;
}
