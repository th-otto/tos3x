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


int fprintf(P(FILE *) fp, P(const char *) fmt _va_alist)
PP(FILE *fp;)
PP(const char *fmt;)
va_dcl
{
	register int ret;
	va_list args;
	
	if (!(fp->_flag & _IOWRT))
		return -1;

	_va_start(args, fmt);
	ret = __doprint(fp, fmt, 0, args);
	_va_end(args);
	if (!(fp->_flag & _IONBUF))
		fflush(fp);
	return ret;
}
