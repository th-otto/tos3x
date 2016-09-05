/***********************************************************************
*
*			p e r r o r   F u n c t i o n
*			-----------------------------
*	Copyright 1982 by Digital Research Inc.  All rights reserved.
*
*	"perror" produces a short error message on stderr describing
*	the last error encountered during a call to the system.
*	It assumes the variable "errno" contains this error.
*	Note: "errno" is set when errors occur, but is not cleared.
*	Note also that some of the UNIX errors are meaningless under 
*	CP/M, and will print an undefined message (defined below).
*	Finally, note that the defines in """errno.h""" will index 
*	appropriately into the sys_errlist table, below.
*
*	Calling sequence:
*		ret = perror(s)
*	Where:
*		ret = errno
*		s   -> a string containing a message printed with
*			the explanatory error message.
*
**************************************************************************/
#include "lib.h"
#include <errno.h>
#include <string.h>

VOID perror(P(const char *) str)
PP(const char *str;)
{
	int err = errno;
	
	if (str && *str)
	{
		fputs(str, stderr);
		fputs(": ", stderr);
	}
	fputs(strerror(err), stderr);
	fputc('\n', stderr);
}
