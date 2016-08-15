/* NOTTYIN: eliminates console input functions from read(). */

#include <osif.h>
#include "lib.h"

/* stubroutine 'tag' */

VOID nottyin(NOTHING)
{
}

static char * __nottyin_msg = "tty input";

size_t _ttyin(P(FD *, fp), P(VOIDPTR, buff), P(size_t, bytes))
PP(FD *, fp;)
PP(VOIDPTR, buff;)
PP(size_t, bytes;)
{
	UNUSED(fp);
	UNUSED(buff);
	UNUSED(bytes);
	_optoff(__nottyin_msg);
	return -1;
}
