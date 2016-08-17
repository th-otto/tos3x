/*****************************************************************************
*
*			f p u t c   F u n c t i o n
*			---------------------------
*	Copyright 1982 by Digital Research Inc.  All rights reserved.
*
*	"fputc" does the inserting of a char into a stream buffer,
*	calling _flsbuf when the buffer fills up.
*	Returns the character put, or FAILURE (-1) if any errors.
*
*	Calling sequence:
*		ret=fputc(ch,s)
*	Where:
*		ch = the char to put
*		s -> the stream (FILE *)
*		ret= ch or FAILURE
*
*****************************************************************************/

#include "lib.h"

int fputc(P(int) ch, P(FILE *) sp)
PP(register char ch;)
PP(register FILE *sp;)
{
	/* if there's room in buf   */
	if (--(sp->_cnt) >= 0)
		return (((int) (*sp->_ptr++ = ch)) & 0377);
	else
		/* o.w. flush & put     */
		return _flsbuf(ch, sp);
}
