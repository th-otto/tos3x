/*
	Copyright 1982
	Alcyon Corporation
	8716 Production Ave.
	San Diego, Ca.  92121
*/

#include "lib.h"

/**
 **	formated print
 **/

int printf(P(const char *) fmt _va_alist)
PP(const char *fmt;)
va_dcl
{
	va_list args;
	register int ret;
	
	_va_start(args, fmt);
	ret = __doprint(stdout, fmt, 0, args);
	_va_end(args);
	return ret;
}
