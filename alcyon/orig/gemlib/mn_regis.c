#include "aeslib.h"

int16_t menu_register(P(int16_t) pid, P(const char *) pstr)
PP(int16_t pid;)
PP(const char *pstr;)
{
	MM_PID = pid;
	MM_PSTR = NO_CONST(pstr);
	return crys_if(MENU_REGISTER);
}
