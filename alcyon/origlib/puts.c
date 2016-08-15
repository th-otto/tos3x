/**************************************************************************
*
*			p u t s   F u n c t i o n
*			-------------------------
*	Copyright 1982 by Digital Research Inc.  All rights reserved.
*
*	"puts" copies a null terminated string to the standard output.
*	It copies a newline char after the end of the string.
*
*	Calling sequence:
*		ret = puts(s)
*	Where:
*		s  = string to put
*		ret = last char output
*
*****************************************************************************/

#include <stdio.h>

int puts(P(register const char *, str))
PP(register const char *, str;)							/* null term string     */
{
	while (*str)						/* for all chars in s       */
		if (putc(*str++, stdout) == -1)	/*   if putc fouls up       */
			return -1;			/*     give up          */
	return putc('\n', stdout);		/* append newline & return  */
}
