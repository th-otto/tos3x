/**************************************************************************
*
*	f p u t s   F u n c t i o n
*	---------------------------
*	Copyright 1982 by Digital Research Inc.  All rights reserved.
*
*	"fputs" copies a null terminated string to a stream file.
*	To be compatible with Unix, it does NOT append a newline.
*
*	Calling sequence:
*		ret = fputs(s,stream)
*	Where:
*		ret = last char out, on error FAILURE
*		s  = string to put
*		stream -> stream (FILE *)
*
*****************************************************************************/

#include <stdio.h>

int fputs(P(const char *) str, P(FILE *) sp)
PP(register char *str;)
PP(register FILE *sp;)
{
	register int rv;						/* return val           */

	rv = 0;							/* init for *s==NULL        */
	while (*str)						/* for all chars in s       */
		if ((rv = putc(*str++, sp)) == EOF)	/*   if putc fouls up       */
			return EOF;			/*     give up          */
	return rv;						/* it worked            */
}
