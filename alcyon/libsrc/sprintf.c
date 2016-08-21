/*
	Copyright 1982
	Alcyon Corporation
	8716 Production Ave.
	San Diego, Ca.  92121
*/

/**
 **	formated print
 **/

#include "lib.h"
#include <string.h>


int sprintf(P(char *) s, P(const char *) fmt _va_alist)
PP(char *s;)
PP(const char *fmt;)
_va_dcl
{
	register int ret;
	FILE *stream;
	va_list args;
	
	stream = (FILE *)&s;
	_va_start(args, fmt);
	ret = __doprint(stream, fmt, 1, args);
	_va_end(args);
	*s = '\0';
	return ret;
}
