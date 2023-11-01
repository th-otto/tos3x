/************************************************************************
*
*			s p r i n t f   F u n c t i o n
*			-------------------------------
*	Copyright 1982 by Digital Research Inc.  All rights reserved.
*
*	"sprintf" prints args specified in format string to a string
*	pointed to by str.  No checks for str overflow are possible.
*	sprintf returns str.
*
*	Calling sequence:
*		s = sprintf(string,fmt,arg1,arg2,...argn);
*	Where:
*		string = place to put info
*		fmt -> a string specifying how arg1-n are to be printed.
*		s = number chars put to string
*
*	10/83 - return num chars printed (like printf & fprintf)   whf
*
**************************************************************************/

#include <osif.h>
#include "lib.h"
#include <stdio.h>

int sprintf(P(char *) str, P(const char *) fmt _va_alist)
PP(char *str;)
PP(const char *fmt;)
_va_dcl
{
	FILE stream; 				/* pseudo stream tab	    */
	register FILE *sp;			/* ptr thereto		    */
	register int rv;			/* return val from _doprt   */

	sp = &stream;				/* point to pseudo stream   */
	sp->_cnt = 32767;			/* assume 'infinite' buf    */
	sp->_ptr = sp->_base = str;		/* stream buf -> string	    */
	sp->_flag = _IOWRT | _IOSTRI;		/* writeable string	    */
	sp->_fd = -1;				/* insure no real i/o	    */
#ifdef __USE_STDARG
	{
	va_list args;
	va_start(args, fmt);
	rv = _doprt(sp, fmt, args);		/* do the print		    */
	va_end(args);
	}
#else
	rv = _doprt(sp, fmt, &va_alist);		/* do the print		    */
#endif
	putc('\0', sp);				/* NULL terminate string    */
	return rv;					/* tell user what happened  */
}
