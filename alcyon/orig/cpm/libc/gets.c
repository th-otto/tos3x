/***********************************************************************
*
*			g e t s   F u n c t i o n
*			-------------------------
*	Copyright 1982 by Digital Research Inc.  All rights reserved.
*
*	"gets" reads a string from the standard input, and returns
*	its argument (NULL on end of file).  The newline at the
*	end of the string is replaced by a EOS.
*
*	Calling sequence:
*		addr = gets(saddr)
*	Where:
*		saddr -> where the string is to go (no bounds check)
*		addr  = saddr if all ok, NULL o.w.
*
*****************************************************************************/

#include "lib.h"

char *gets(P(char *) str)
PP(register char *str;)
{
	register int c;							/* char to test for eof     */
	char *sav;							/* sav pointer for return   */

	sav = str;							/* remember this        */
	while ((c = getc(stdin)) != EOF &&	/* while everything ok      */
		   c != '\n')					/*   and not end of line    */
		*str++ = c;						/*     store it         */
	*str = '\0';						/* clean up string      */
	if (c == EOF)					/* if not cool          */
		return NULL;				/*   then tell them so      */
	return sav;						/* tell them is cool        */
}
