/***********************************************************************
*
*			f g e t s   F u n c t i o n
*			---------------------------
*	Copyright 1982 by Digital Research Inc.  All rights reserved.
*
*	"fgets" reads a string from a stream file (up to n-1 chars), and 
*	returns pointer s (NULL on end of file).  The newline at the
*	end of line is included, NOT replaced, and the string is terminated
*	by a NULL.
*
*	Calling sequence:
*		addr = fgets(saddr,n,stream)
*	Where:
*		saddr -> where the string is to go (no bounds check)
*		n     = max # chars (inc. NULL) for saddr (assumed > 0)
*		stream-> where to get from
*		addr  = saddr if all ok, NULL o.w.
*
*****************************************************************************/

#include <stdio.h>


char *fgets(P(char *) str, P(int) maxc, P(FILE *)sp)
PP(register char *str;)
PP(register int maxc;)							/* max size of string       */
PP(register FILE *sp;)							/* where to get from        */
{
	register int c;							/* char to test for eof     */
	register char *sav;						/* sav pointer for return   */

	sav = str;							/* remember this        */
#ifndef __ALCYON__
	c = 0;
#endif
	while (--maxc > 0 &&				/* while there's still room */
									   /*   for getc and NULL      */
		   (c = getc(sp)) != EOF)	/*   and read_char ok       */
	{
		*str++ = c;						/*   store it           */
		if (c == '\n')					/*   if end of line     */
			break;						/*     stop the presses     */
	}
	*str = '\0';						/* clean up string      */
	if (c == EOF)		/* BUG: c may be used uninitialzed */
		return NULL;				/*   then tell them so      */
	return sav;
}
