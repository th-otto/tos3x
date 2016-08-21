/**************************************************************************
*
*			s s c a n f   F u n c t i o n
*			-----------------------------
*	Copyright 1982 by Digital Research Inc.  All rights reserved.
*
*	"sscanf" scans the given string for items specified, and assigns
*	them to user supplied variables (via pointers to these vars).
*
*	Calling sequence:
*		ret = sscanf(str,fmt,p1,p2,...)
*	Where:
*		ret =  the number of items successfully matched & assigned
*			EOF returned if encountered on input
*		str -> a (null terminated) string to scan
*		fmt -> a string specifying how to parse the input
*		p1,p2,... -> where matched items get stored
*
*****************************************************************************/

#include "lib.h"
#include <string.h>

int	sscanf(P(const char *) str, P(const char *) fmt _va_alist)
PP(const char *str;)
PP(const char *fmt;)
_va_dcl
{
	register FILE *sp;
	FILE spbuf;
	char locbuf[BUFSIZ];			/* set up a local buffer    */

	sp = &spbuf;				/* set up a pseudo stream   */
	sp->_fd = -1;
	sp->_flag = _IOREAD | _IOSTRI;		/* readable string	    */
	sp->_cnt = strlen(str) + 1;		/* allow doscan to read NULL*/
	strcpy(locbuf, str);			/* copy to safe place	    */
	sp->_base = sp->_ptr = locbuf;
#ifdef __USE_STDARG
	{
	va_list args;
	va_start(args, fmt);
	return _doscan(sp, fmt, args);
	}
#else
	return _doscan(sp, fmt, &va_alist);
#endif
}
