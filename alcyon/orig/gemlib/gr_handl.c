#include "aeslib.h"

int16_t graf_handle(P(int16_t *) pwchar, P(int16_t *) phchar, P(int16_t *) pwbox, P(int16_t *) phbox)
PP(int16_t *pwchar;)
PP(int16_t *phchar;)
PP(int16_t *pwbox;)
PP(int16_t *phbox;)
{
	crys_if(GRAF_HANDLE);
	*pwchar = GR_WCHAR;
	*phchar = GR_HCHAR;
	*pwbox = GR_WBOX;
	*phbox = GR_HBOX;
	return RET_CODE;
}
