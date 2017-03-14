#include "aeslib.h"

int16_t form_alert(P(int16_t) defbut, P(const char *) astring)
PP(int16_t defbut;)
PP(const char *astring;)
{
	FM_DEFBUT = defbut;
	FM_ASTRING = NO_CONST(astring);
	return crys_if(FORM_ALERT);
}
