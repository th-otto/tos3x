#include "aeslib.h"

int16_t fsel_input(P(char *) pipath, P(char *) pisel, P(int16_t *) pbutton)
PP(char *pipath;)
PP(char *pisel;)
PP(int16_t *pbutton;)
{
	FS_IPATH = pipath;
	FS_ISEL = pisel;
	crys_if(FSEL_INPUT);
	*pbutton = FS_BUTTON;
	return RET_CODE;
}
