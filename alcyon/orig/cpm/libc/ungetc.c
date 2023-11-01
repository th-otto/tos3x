/**************************************************************************
*
*			u n g e t c   F u n c t i o n
*			-----------------------------
*	Copyright 1982 by Digital Research Inc.  All rights reserved.
*
*	"ungetc" puts a char back into its (read only) stream.  It's
*	guaranteed to work for at least one char, provided the
*	stream is buffered and something has been read.
*
*	Calling sequence:
*		ret = ungetc(c,stream)
*	Where:
*		ret = c if it worked, FAILURE o.w.
*		c   = char to push back
*		stream -> (FILE *) place to push back to
*
*****************************************************************************/

#include "lib.h"

int ungetc(P(int) ch, P(FILE *) sp)
PP(register int ch;)
PP(register FILE *sp;)							/* stream to unget to       */
{
	if (ch == EOF)						/* no allowed if EOF        */
		return EOF;
	if (sp->_flag & _IOREAD				/* if file is read able     */
		&& sp->_base != NULL			/*   and something's read   */
		&& sp->_ptr > sp->_base)		/*   and buffer not empty   */
	{
		*--sp->_ptr = ch;				/* back up buffer & store   */
		sp->_cnt++;						/* there's one more byte    */
		return ch;					/* it worked!           */
	}
	return EOF;					/* it didn't work...        */
}
