/************************************************************************
*
*			p r i n t f   F u n c t i o n
*			-----------------------------
*	Copyright 1982 by Digital Research Inc.  All rights reserved.
*
*	"printf" prints args specified in format string to stdout.
*
*	Calling sequence:
*		nchrs = printf(fmt,arg1,arg2,...argn);
*	Where:
*		nchrs = # chars output by printf
*		fmt -> a string specifying how arg1-n are to be printed.
*
**************************************************************************/

#define printf std_printf
#define fprintf std_fprintf

#include "lib.h"

#undef printf
#undef fprintf

int printf PROTO((const char *fmt, ...));

int printf(P(const char *) fmt _va_alist)
PP(const char *fmt;)
_va_dcl
{
#ifdef __USE_STDARG
	va_list args;
	va_start(args, fmt);
	return _doprt(stdout, fmt, (char *)args);
#else
	return _doprt(stdout, fmt, (char *)&va_alist);
#endif
}
