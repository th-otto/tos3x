/* OPTOFF.C: prints a message in case someone tries to use a part of the    */
/*	RTL which was OPTIONally linked out.				    */

#include "lib.h"
#include <osif.h>
#include <string.h>
#include <stdlib.h>

VOID _optoff(P(const char *) msg)
PP(const char *msg;)
{
	__OSIF(C_WRITESTR, "C RTL - program not linked for ");
	__OSIF(C_WRITESTR, msg);
	__OSIF(C_WRITESTR, "\r\nProgram terminating\r\n");
	_exit(-1);
}
