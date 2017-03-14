#include "aeslib.h"

int16_t rsrc_rcfix(P(RSHDR *) header)
PP(RSHDR *header;)
{
	RS_HEADER = header;
	return crys_if(RSRC_RCFIX);
}
