/************************************************************************
*
*			f p r i n t f   F u n c t i o n
*			-------------------------------
*	Copyright 1982 by Digital Research Inc.  All rights reserved.
*
*	"fprintf" prints args specified in format string to a stream file.
*
*	Calling sequence:
*		nchrs = fprintf(fmt,arg1,arg2,...argn);
*	Where:
*		nchrs = number of chars output
*		fmt -> a string specifying how arg1-n are to be printed.
*
**************************************************************************/

#define printf std_printf
#define fprintf std_fprintf

#include "lib.h"

#undef printf
#undef fprintf

int fprintf PROTO((FILE *sp, const char *fmt, ...));

int fprintf(P(FILE *, sp), P(const char *, fmt) _va_alist)
PP(FILE *, sp;)
PP(const char *, fmt;)
_va_dcl
{
#ifdef __USE_STDARG
	va_list args;
	va_start(args, fmt);
	return _doprt(sp, fmt, (char *)args);
#else
	return _doprt(sp, fmt, (char *)&va_alist);
#endif
}
