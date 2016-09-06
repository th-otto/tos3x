/**************************************************************************
*
*			f s e e k   F u n c t i o n
*			---------------------------
*	Copyright 1982 by Digital Research Inc.  All rights reserved.
*
*	"fseek" sets the read/write pointer of a stream to an arbitrary
*	offset.
*
*	Calling sequence:
*		ret = fseek(sp,offset,sense)
*	Where:
*		sp -> (FILE *) stream to seek on
*		offset = signed number of bytes
*		sense =	0  =>	offset from beginning of file
*			1  =>	offset from current position
*			2  =>	from end of file
*		ret = 0 for success, -1 for failure
*
*****************************************************************************/

#include "lib.h"
#include <stdio.h>

int fseek(P(FILE *) sp, P(long) offs, P(int) sense)
PP(register FILE *sp;)
PP(long offs;)
PP(int sense;)
{
	long p;

	if (fflush(sp) < 0)
		return EOF;
	p = lseek(fileno(sp), offs, sense);
	sp->_flag &= ~_IOEOF;				/* any seek clears EOF flag */
	return p == (long) -1 ? -1 : 0;
}
