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

#include <stdio.h>

int fflush(P(FILE *) sp)
PP(register FILE *sp;)							/* stream to flush      */
{
	register int n;							/* num written          */
	register int ns;						/* num sposed to be written */

	if ((sp->_flag & (_IONBF | _IOWRT)) == _IOWRT	/* does it have a wrt buf?  */
		&& (ns = (long) sp->_ptr - (long)sp->_base) > 0)	/*  and does buf need wrt? */
	{
		n = write(fileno(sp), sp->_base, ns);	/* do it            */
		if (ns != n)					/* did they all git writ?   */
		{
			sp->_flag |= _IOERR;		/*   this stream no good    */
			return -1;				/*   let em know        */
		}
	}
	if (sp->_flag & _IOWRT)				/* is this a writable file? */
	{
		if (sp->_base != NULL)			/* written to already?      */
		{
			if (sp->_flag & _IONBF)	/* is this a nobuf stream?  */
				sp->_cnt = 1;
			else
				sp->_cnt = BUFSIZ - 1;	/* standard size        */
		}
	} else
	{									/* is a readable file       */
		lseek(fileno(sp), (long) - (sp->_cnt), SEEK_CUR);	/* back up cur position ptr */
		sp->_cnt = 0;					/* zap out count        */
	}
	sp->_ptr = sp->_base;				/* reset buf */
	return 0;
}
