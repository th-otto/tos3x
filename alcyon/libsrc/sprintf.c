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


#ifdef __USE_STDARG
int sprintf(char *s, const char *fmt, ...)
#else
int sprintf(P(char *) s, P(const char *) fmt, va_alist)
PP(char *s;)
PP(const char *fmt;)
va_dcl
#endif
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
