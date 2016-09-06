/****************************************************************************
*
*			f f l u s h   F u n c t i o n
*			-----------------------------
*	Copyright 1982 by Digital Research Inc.  All rights reserved.
*
*	'fflush' causes any buffered data in a stream to be written out
*	to the file.  The stream remains open.
*
*	Calling sequence:
*		ret = fflush(stream)
*	Where:
*		stream is a (FILE *)
*		ret = SUCCESS or FAILURE (I/O error)
*
*	9/83 fix fseek(stream,xx,1) bug   whf
*
*****************************************************************************/

#include "lib.h"
#include <errno.h>

int fflush(P(FILE *) sp)
PP(register FILE *sp;)							/* stream to flush      */
{
	register int n;							/* num written          */
	register int ns;						/* num sposed to be written */
	register int ii;

	if (sp == NULL)
	{
		n = 0;
		for (ii = 0; ii < MAXFILES; ii++)
			n |= fflush(&_iob[ii]);
		return n;
	}
	
	if (!__validfp(sp))
	{
		__set_errno(EINVAL);
		return EOF;
	}
	if (sp->_flag & _IOWRTN)	/* does it have a wrt buf?  */
	{
		if ((ns = (long) sp->_ptr - (long)sp->_base) > 0)	/*  and does buf need wrt? */
		{
			n = write(fileno(sp), sp->_base, ns);	/* do it            */
			if (ns != n)					/* did they all git writ?   */
			{
				sp->_flag |= _IOERR;		/*   this stream no good    */
				return EOF;
			}
		}
	} else if (sp->_flag & _IOREDN)
	{									/* is a readable file       */
		if (sp->_cnt > 0)
			lseek(fileno(sp), (long) - (sp->_cnt), SEEK_CUR);	/* back up cur position ptr */
	}
	sp->_cnt = 0;					/* zap out count        */
	sp->_ptr = sp->_base;				/* reset buf */
	sp->_flag &= ~(_IOWRTN | _IOREDN);
	return 0;
}
