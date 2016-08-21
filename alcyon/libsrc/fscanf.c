/**************************************************************************
*
*			f s c a n f   F u n c t i o n
*			-----------------------------
*	Copyright 1982 by Digital Research Inc.  All rights reserved.
*
*	"fscanf" scans the given stream for items specified, and assigns
*	them to user supplied variables (via pointers to these vars).
*
*	Calling sequence:
*		ret = fscanf(sp,fmt,p1,p2,...)
*	Where:
*		ret =  the number of items successfully matched & assigned
*			EOF returned if encountered on input
*		sp  -> (FILE *) stream pointer
*		fmt -> a string specifying how to parse the input
*		p1,p2,... -> where matched items get stored
*
*****************************************************************************/

#include "lib.h"

int fscanf(P(FILE *) sp, P(const char *) fmt _va_alist)
PP(FILE *sp;)
PP(const char *fmt;)
_va_dcl
{
#ifdef __USE_STDARG
	va_list args;
	va_start(args, fmt);
	return _doscan(sp, fmt, args);
#else
	return _doscan(sp, fmt, &va_alist);
#endif
}
