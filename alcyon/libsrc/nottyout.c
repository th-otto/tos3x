/* NOTTYIN: eliminates console input functions from read(). */

#include <osif.h>
#include "lib.h"
#include <option.h>

/* stubroutine 'tag' */

VOID nottyout(NOTHING)
{
}

static char const __nottyout_msg[] = "tty output";

int _ttyout(P(const char *) buff)
PP(const char *buff;)
{
	UNUSED(buff);
	_optoff(__nottyout_msg);
	return -1;
}

int _lstout(P(const char *) buff)
PP(const char *buff;)
{
	UNUSED(buff);
	_optoff(__nottyout_msg);
	return -1;
}
