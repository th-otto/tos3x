/* OPTOFF.C: prints a message in case someone tries to use a part of the    */
/*	RTL which was OPTIONally linked out.				    */

#include "lib.h"
#include <osif.h>
#include <string.h>
#include <stdlib.h>

VOID _optoff(P(const char *, msg))
PP(const char *, msg;)
{
	char buf[200];						/* ought to be big enough   */

	strcpy(buf, "C RTL - program not linked for ");
	strcat(buf, msg);
	strcat(buf, "\r\nProgram terminating\r\n$");
	__BDOS(C_WRITESTR, (long)(buf));
	_exit(-1);
}
