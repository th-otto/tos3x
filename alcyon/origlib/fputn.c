/************************************************************************
*
*			f p u t n   F u n c t i o n
*			---------------------------
*	Copyright 1983 by Digital Research Inc.  All rights reserved.
*
*	'fputn()' writes 'n' chars to a buffered file.  It optimizes
*	for 'non-buffered' (_IONBUF) output.
*
*	Calling sequence:
*		rv = fputn(cp,n,stream)
*	Where:
*		cp -> chars to be put
*		n  =  num chars to be put
*		stream -> where its going (FILE *)
*		rv = SUCCESS if write ok, FAILURE (-1 int) o.w.
*
*****************************************************************************/

#include <stdio.h>

int fputn(P(register const char *) buf, P(register int) num, P(register FILE *) sp)
PP(register const char *buf;)				/* chars to be written      */
PP(register int num;)						/* num chars to be written  */
PP(register FILE *sp;)						/* stream to write to       */
{
	if (sp->_flag & _IONBUF)			/* Non-buffered file?       */
	{
		sp->_cnt = 0;					/* Always force to zero     */
		if (num != write(fileno(sp), buf, num))	/* try to write         */
		{
			/* mark this file and complain */
			sp->_flag |= _IOERR;
			return -1;
		}
		return 0;
	}
	
	while (num--)						/* Buffered output      */
		if (-1 == fputc(*buf++, sp))	/* try to put in buf        */
			return -1;
	return 0;
}
