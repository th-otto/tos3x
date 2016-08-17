/* NOTTYIN: eliminates console input functions from read(). */

#include <osif.h>
#include "lib.h"
#include <option.h>

/* stubroutine 'tag' */

VOID nottyin(NOTHING)
{
}

static const char * __nottyin_msg = "tty input";

size_t _ttyin(P(FD *) fp, P(VOIDPTR) buff, P(size_t) bytes)
PP(FD *fp;)
PP(VOIDPTR buff;)
PP(size_t bytes;)
{
	UNUSED(fp);
	UNUSED(buff);
	UNUSED(bytes);
	_optoff(__nottyin_msg);
	return -1;
}
