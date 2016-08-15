/**************************************************************************
*
*			f g e t c   F u n c t i o n
*			---------------------------
*	Copyright 1982 by Digital Research Inc.  All rights reserved.
*
*	Function "fgetc" does the yanking of a char out of the stream.
*
*	Calling sequence:
*		ch = fgetc(s)
*	Where:
*		ch = a (int) character (-1 on EOF)
*		s -> a stream file (FILE *)
*
*****************************************************************************/

#include "lib.h"
#define CMASK 0xFF

int fgetc(P(FILE *, sp))
PP(register FILE *, sp;)							/* stream pointer       */
{
	if (--sp->_cnt >= 0)				/* any chars left in buf?   */
		return (((int) *sp->_ptr++) & CMASK);	/* return & advance ptr ifso */
	return _filbuf(sp);			/* o.w. whatever happens    */
}
