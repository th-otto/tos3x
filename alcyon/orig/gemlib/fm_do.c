#include "aeslib.h"

int16_t form_do(P(OBJECT *) form, P(int16_t) start)
PP(OBJECT *form;)
PP(int16_t start;)
{
	FM_FORM = form;
	FM_START = start;
	return crys_if(FORM_DO);
}
