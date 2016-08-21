/**************************************************************************
*
*			s c a n f   F u n c t i o n
*			---------------------------
*	Copyright 1982 by Digital Research Inc.  All rights reserved.
*
*	"scanf" scans the standard input for items specified, and assigns
*	them to user supplied variables (via pointers to these vars).
*
*	Calling sequence:
*		ret = scanf(fmt,p1,p2,...)
*	Where:
*		ret = the number of items successfully matched & assigned
*			EOF returned if encountered on input
*		fmt -> a string specifying how to parse the input
*		p1,p2,... -> where matched items get stored
*
*****************************************************************************/

#include "lib.h"

int scanf(P(const char *) fmt _va_alist)
PP(const char *fmt;)
_va_dcl
{
#ifdef __USE_STDARG
	va_list args;
	va_start(args, fmt);
	return _doscan(stdout, fmt, args);
#else
	return _doscan(stdin, fmt, &va_alist);
#endif
}
